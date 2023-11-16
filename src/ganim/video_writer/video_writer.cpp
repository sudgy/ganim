#include "video_writer.hpp"

#include "libav.h"

#include <iostream>

using namespace ganim;

/// @private
struct VideoWriter::Impl {
    int width = 0;
    int height = 0;
    int fps = 0;
    int frame = 0;
    const AVOutputFormat* oformat = nullptr;
    AVFormatContext* fcontext = nullptr;
    AVCodecContext* ccontext = nullptr;
    SwsContext* sws_context = nullptr;
    AVFrame* video_frame = nullptr;
    AVPacket* packet = nullptr;
};

VideoWriter::VideoWriter(std::string filename, int width, int height, int fps) :
    M_impl(std::make_unique<Impl>())
{
    M_impl->width = width;
    M_impl->height = height;
    M_impl->fps = fps;

    M_impl->oformat = av_guess_format(nullptr, filename.c_str(), nullptr);
    if (!M_impl->oformat) {
        throw std::runtime_error("Unable to create output format");
    }
    auto error = avformat_alloc_output_context2(
        &M_impl->fcontext, M_impl->oformat, nullptr, filename.c_str());
    if (error) {
        throw std::runtime_error("Unable to create output context");
    }
    auto codec = avcodec_find_encoder(M_impl->oformat->video_codec);
    if (!codec) {
        throw std::runtime_error("Unable to create codec");
    }
    auto stream = avformat_new_stream(M_impl->fcontext, codec);
    if (!stream) {
        throw std::runtime_error("Unable to create stream");
    }
    M_impl->ccontext = avcodec_alloc_context3(codec);
    if (!M_impl->ccontext) {
        throw std::runtime_error("Unable to create codec context");
    }
    M_impl->ccontext->log_level_offset = 1;

    stream->codecpar->codec_id = M_impl->oformat->video_codec;
    stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    stream->codecpar->width = width;
    stream->codecpar->height = height;
    stream->codecpar->format = AV_PIX_FMT_YUV420P;
    stream->codecpar->bit_rate = 4000000;
    avcodec_parameters_to_context(M_impl->ccontext, stream->codecpar);

    M_impl->ccontext->time_base = AVRational(1, 1);
    M_impl->ccontext->max_b_frames = 1;
    M_impl->ccontext->gop_size = 10;
    M_impl->ccontext->framerate = AVRational(fps, 1);
    av_opt_set(M_impl->ccontext, "preset", "ultrafast", 0);
    avcodec_parameters_from_context(stream->codecpar, M_impl->ccontext);

    error = avcodec_open2(M_impl->ccontext, codec, nullptr);
    if (error < 0) {
        throw std::runtime_error("Unable to open codec");
    }
    error = avio_open(&M_impl->fcontext->pb, filename.c_str(), AVIO_FLAG_WRITE);
    if (error < 0) {
        throw std::runtime_error("Unable to open file");
    }
    error = avformat_write_header(M_impl->fcontext, nullptr);
    if (error < 0) {
        throw std::runtime_error("Unable to write header");
    }

    M_impl->sws_context = sws_getContext(
        width, height, AV_PIX_FMT_RGB24,
        width, height, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, nullptr, nullptr, nullptr
    );

    M_impl->video_frame = av_frame_alloc();
    M_impl->video_frame->format = AV_PIX_FMT_YUV420P;
    M_impl->video_frame->width = width;
    M_impl->video_frame->height = height;
    error = av_frame_get_buffer(M_impl->video_frame, 32);
    if (error < 0) {
        throw std::runtime_error("Unable to allocate frame");
    }

    M_impl->packet = av_packet_alloc();
    M_impl->packet->data = nullptr;
    M_impl->packet->size = 0;
    M_impl->packet->flags |= AV_PKT_FLAG_KEY;
}

VideoWriter::~VideoWriter()
{
    finish();
}
VideoWriter::VideoWriter(VideoWriter&&)=default;
VideoWriter& VideoWriter::operator=(VideoWriter&&)=default;

void VideoWriter::write_frame(std::span<std::uint8_t> image)
{
    if (!M_impl) {
        throw std::logic_error(
            "Trying to write a frame to an already finished VideoWriter");
    }
    if (ssize(image) != M_impl->width * M_impl->height * 3) {
        throw std::invalid_argument(
            "The image passed to VideoWriter::write_frame has an incorrect "
            "size.");
    }
    int linesize = 3*M_impl->width;
    auto data = image.data();
    sws_scale(M_impl->sws_context, &data, &linesize, 0, M_impl->height,
            M_impl->video_frame->data, M_impl->video_frame->linesize);
    // I honestly have no idea where the 90000 comes from, I hope that's not
    // tied to the framerate or bitrate somehow
    M_impl->video_frame->pts = (1.0 / M_impl->fps) * 90000 * (M_impl->frame++);

    auto error = avcodec_send_frame(M_impl->ccontext, M_impl->video_frame);
    if (error < 0) {
        throw std::runtime_error("Unable to send frame");
    }

    receive_packets();
}

void VideoWriter::receive_packets()
{
    auto ret = 0;
    while (ret == 0) {
        ret = avcodec_receive_packet(M_impl->ccontext, M_impl->packet);
        if (ret == 0) {
            av_interleaved_write_frame(M_impl->fcontext, M_impl->packet);
        }
    }
}

void VideoWriter::finish()
{
    if (!M_impl) return;
    avcodec_send_frame(M_impl->ccontext, nullptr);
    receive_packets();

    av_write_trailer(M_impl->fcontext);
    int error = avio_close(M_impl->fcontext->pb);
    if (error < 0) {
        throw std::runtime_error("Failed to close file");
    }

    if (M_impl->packet) av_packet_free(&M_impl->packet);
    if (M_impl->video_frame) av_frame_free(&M_impl->video_frame);
    if (M_impl->ccontext) avcodec_free_context(&M_impl->ccontext);
    if (M_impl->fcontext) avformat_free_context(M_impl->fcontext);
    if (M_impl->sws_context) sws_freeContext(M_impl->sws_context);

    M_impl.reset();
}
