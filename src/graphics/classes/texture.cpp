#include "texture.h"
#include "stb_image.h"

void Texture::loadFile(const std::string& file_name, const TextureOptions& opts) {
    int w, h, nr_channels;
    stbi_set_flip_vertically_on_load(opts.flip_vertical);
    unsigned char* data = stbi_load(file_name.c_str(), &w, &h, &nr_channels, 0);

    if (!data) {
        return;
    }

    bind();
    m_width = w;
    m_height = h;
    GLenum format = GL_RGBA;

    setParams(opts);

    switch (nr_channels) {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        default:
            format = GL_RGBA;
            break;
    }

    glTexImage2D(m_target, 0, format, m_width, m_height, 0, format, opts.data_type, data);

    if (opts.mipmap) {
        glGenerateMipmap(m_target);
    }

    stbi_image_free(data);
}

void Texture::setParams(const TextureOptions& opts) {
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, opts.wrap);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, opts.wrap);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_R, opts.wrap);

    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, opts.min_filter);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, opts.mag_filter);
}

void Texture::bind(uint32_t unit) const {
    if (m_id != 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(m_target, m_id);
    }
}

void Texture::unbind() const {
    glBindTexture(m_target, 0);
}

void Texture::destroy() {
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}
