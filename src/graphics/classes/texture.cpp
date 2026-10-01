#include "texture.h"
#include "stb_image.h"

void Texture::loadFile(const std::string& file_name) {
    int w, h, nr_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(file_name.c_str(), &w, &h, &nr_channels, 0);

    if (data) {
        this->bind();
        m_width = w;
        m_height = h;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLenum format = (nr_channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
}

void Texture::bind() const {
    if (m_id != 0) {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::destroy() {
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}
