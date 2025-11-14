#include "image.hpp"

#include "ganim/util/stb_image.h"

using namespace ganim;

Image::Image(const std::string& filename)
{
    int width, height, channels;
    auto image = stbi_load(filename.c_str(), &width, &height, &channels, 4);
    if (image == nullptr) {
        throw std::runtime_error("Error loading image " + filename);
    }
    M_texture = gl::Texture(image, width, height);
    stbi_image_free(image);
    // Assuming a 1920/1080 resolution for the default size
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
    set_texture(M_texture);
}
