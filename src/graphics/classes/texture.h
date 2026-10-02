#pragma once
#include "../gfx.h"

#include <string>

struct TextureOptions {
    GLint min_filter { GL_LINEAR_MIPMAP_LINEAR };
    GLint mag_filter { GL_LINEAR_MIPMAP_LINEAR };
    GLint wrap { GL_REPEAT };
    GLenum data_type { GL_UNSIGNED_BYTE };
    bool mipmap { true };
    bool flip_vertical { true };

    // need?
    // bool s_rgb { false };
};

class Texture {
  private:
    GLuint m_id {};
    GLuint m_target { GL_TEXTURE_2D };
    int m_width {}, m_height {};

  public:
    Texture(GLuint target = GL_TEXTURE_2D)
        : m_target { target } {
        glGenTextures(1, &m_id);
    }

    ~Texture() { destroy(); }

    Texture(Texture&& t) noexcept
        : m_id { t.m_id }, m_width { t.m_width }, m_height { t.m_height } {
        t.m_id = 0;
        t.m_width = 0;
        t.m_height = 0;
    }

    Texture& operator=(Texture&& t) noexcept {
        if (&t == this)
            return *this;

        this->destroy();
        m_id = t.m_id;
        m_width = t.m_width;
        m_height = t.m_height;

        t.m_id = 0;
        t.m_width = 0;
        t.m_height = 0;

        return *this;
    }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void loadFile(const std::string& file_name, const TextureOptions& opts = {});
    void bind(uint32_t unit = 0) const;
    void unbind() const;
    void destroy();

    int id() const { return m_id; }
    int width() const { return m_width; }
    int height() const { return m_height; }

  private:
    void setParams(const TextureOptions& opts);

};
