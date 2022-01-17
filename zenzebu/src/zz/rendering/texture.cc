#define STB_IMAGE_IMPLEMENTATION

#include "zz/log.h"

#include "glad/glad.h"
#include "texture.h"
#include "stb_image.h"

using namespace zz::render;

texture::texture(std::string filename, bool gen_mipmaps) {
    int w, h, nch;
    unsigned char *data = stbi_load(filename.c_str(), &w, &h, &nch, 0);

    if (data) init(w, h, data, gen_mipmaps);
    else ZZ_CORE_ERROR("Failed to load image {}", filename);

    stbi_image_free(data);
}

texture::texture(int width, int height, unsigned char *data, bool gen_mipmaps) {
    init(width, height, data, gen_mipmaps);
}

texture::~texture() {
    ZZ_CORE_INFO("deleting texture");
    glDeleteTextures(1, (unsigned int *) &id);
}

void texture::init(int w, int h, unsigned char *data, bool gen_mipmaps) {
    glGenTextures(1, (unsigned int *) &id);

    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (gen_mipmaps) glGenerateMipmap(GL_TEXTURE_2D);
}

void texture::use() {
    glBindTexture(GL_TEXTURE_2D, id);
}