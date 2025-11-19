#include "video.hpp"

#include "ganim/gl/gl.hpp"
#include "ganim/video_writer/libav.h"

using namespace ganim;

struct Video::Impl {
    AVFormatContext* fcontext = nullptr;
    AVPacket* packet = nullptr;
    AVCodecContext* ccontext = nullptr;
    AVFrame* frame = nullptr;
    AVFrame* rgb_frame = nullptr;
    SwsContext* sws_context = nullptr;
    int video_index = -1;
    AVRational time_base;
    std::int64_t next_pts = 0;

    bool first_frame = true;

    gl::Texture texture;
};

Video::Video(const std::string& filename)
:   M_impl(std::make_unique<Impl>())
{
    M_impl->packet = av_packet_alloc();
    if (!M_impl->packet) {
        throw std::runtime_error("Unable to allocate packet");
    }
    M_impl->fcontext = avformat_alloc_context();
    if (!M_impl->fcontext) {
        throw std::runtime_error("Unable to create format context");
    }
    avformat_open_input(&M_impl->fcontext, filename.c_str(), nullptr, nullptr);
    if (!M_impl->fcontext) {
        throw std::runtime_error("Unable to open file " + filename);
    }
    avformat_find_stream_info(M_impl->fcontext, nullptr);
    auto codec = (const AVCodec*)nullptr;
    auto codec_params = (AVCodecParameters*)nullptr;
    for (auto i = 0U; i < M_impl->fcontext->nb_streams; ++i) {
        auto this_codec_params = M_impl->fcontext->streams[i]->codecpar;
        if (this_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
            M_impl->time_base = M_impl->fcontext->streams[i]->time_base;
            codec = avcodec_find_decoder(this_codec_params->codec_id);
            codec_params = this_codec_params;
            M_impl->video_index = i;
        }
    }
    if (!codec) {
        throw std::runtime_error("Unable to create codec");
    }
    M_impl->ccontext = avcodec_alloc_context3(codec);
    if (!M_impl->ccontext) {
        throw std::runtime_error("Unable to create codec context");
    }
    avcodec_parameters_to_context(M_impl->ccontext, codec_params);
    M_impl->frame = av_frame_alloc();
    M_impl->rgb_frame = av_frame_alloc();

    if (avcodec_open2(M_impl->ccontext, codec, nullptr) < 0) {
        throw std::runtime_error("Unable to open codec");
    }
    add_updater([this]{update();});
}

void Video::update()
{
    if (!is_visible()) return;
    auto middle_pts = static_cast<int64_t>(double(M_frames) / get_fps()
        * M_impl->time_base.den / M_impl->time_base.num);
    auto end_pts = static_cast<int64_t>((double(M_frames) + 0.5) / get_fps()
        * M_impl->time_base.den / M_impl->time_base.num);
    ++M_frames;
    // Video is playing slowly enough that we need to stay on the same frame
    if (M_impl->next_pts > end_pts) return;

    // Keep getting frames until the next one is what we should be showing
    // (this will only run once if the input and output fps are synced)
    while (true) {
        auto ret = avcodec_receive_frame(M_impl->ccontext, M_impl->frame);
        while (ret == AVERROR(EAGAIN)) {
            if (!send_packet()) {
                set_visible(false);
                return;
            }
            ret = avcodec_receive_frame(M_impl->ccontext, M_impl->frame);
        }
        if (ret == AVERROR_EOF) {
            set_visible(false);
        }
        else if (ret < 0) {
            throw std::runtime_error("Error receiving a frame during decoding");
        }
        if (M_impl->frame->duration == 0) {
            throw std::runtime_error(
                "The current algorithm I'm using for video timing doesn't "
                "work for videos that don't have frame durations.  Let me "
                "see this video so I can try to find an alternative.  But "
                "for now, I can't play this video.");
        }
        auto frame_pts = M_impl->frame->pts;
        auto next_pts = frame_pts + M_impl->frame->duration;
        if (next_pts < middle_pts) continue;
        auto frame_dif = std::abs(frame_pts - middle_pts);
        auto next_dif = std::abs(next_pts - middle_pts);
        if (frame_dif < next_dif) {
            M_impl->next_pts = next_pts;
            break;
        }
    }

    auto width = M_impl->frame->width;
    auto height = M_impl->frame->height;
    if (!M_impl->sws_context) {
        M_impl->sws_context = sws_getContext(
            width, height, AV_PIX_FMT_YUV420P,
            width, height, AV_PIX_FMT_RGB24,
            SWS_BILINEAR, nullptr, nullptr, nullptr
        );
    }
    sws_scale_frame(M_impl->sws_context, M_impl->rgb_frame, M_impl->frame);

    auto data = M_impl->rgb_frame->data[0];
    auto linesize = M_impl->rgb_frame->linesize[0];
    auto byte_size = linesize / width;
    auto format = GL_RGB;
    if (byte_size == 4) format = GL_RGBA;
    else if (byte_size == 1) format = GL_RED;
    else if (byte_size != 3) {
        throw std::runtime_error(std::format(
            "Unknown byte size {} encountered while decoding.", byte_size));
    }
    if (M_impl->first_frame) {
        M_impl->first_frame = false;
        glBindTexture(GL_TEXTURE_2D, M_impl->texture);
        glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
        );
        glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
        );
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
            format, GL_UNSIGNED_BYTE, data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        auto w = width / 240.0f;
        auto h = height / 240.0f;
        set_vertices(
            {{ w,  h, 0},
             { w, -h, 0},
             {-w, -h, 0},
             {-w,  h, 0}},
             std::vector<unsigned>{0, 1, 2, 0, 2, 3}
        );
        set_texture_vertices(
            {{ 0,  0},
             { 0, +1},
             {-1, +1},
             {-1,  0}}
        );
        set_texture(M_impl->texture);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, M_impl->texture);
        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0, width, height,
            format, GL_UNSIGNED_BYTE, data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

bool Video::send_packet()
{
    while (av_read_frame(M_impl->fcontext, M_impl->packet) >= 0) {
        if (M_impl->packet->stream_index == M_impl->video_index) {
            auto ret = avcodec_send_packet(M_impl->ccontext,M_impl->packet);
            av_packet_unref(M_impl->packet);
            if (ret < 0) {
                throw std::runtime_error(
                    "Error sending a packet during decoding");
            }
            return true;
        }
        av_packet_unref(M_impl->packet);
    }
    return false;
}

Video::~Video()
{
    avformat_close_input(&M_impl->fcontext);
    av_packet_free(&M_impl->packet);
    av_frame_free(&M_impl->frame);
    av_frame_free(&M_impl->rgb_frame);
    avcodec_free_context(&M_impl->ccontext);
    sws_freeContext(M_impl->sws_context);
    M_impl.reset();
}

Video::Video(Video&&) noexcept=default;
Video& Video::operator=(Video&&) noexcept=default;
