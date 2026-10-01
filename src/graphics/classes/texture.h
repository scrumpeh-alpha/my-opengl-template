#pragma once
#include "../gfx.h"

#include <string>

class Texture {
  private:
    GLuint m_id {};
    int m_width {}, m_height {};

  public:
    Texture() { glGenTextures(1, &m_id); }
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

    void loadFile(const std::string& file_name);
    void bind() const;
    void unbind() const;
    void destroy();

    int width() const { return m_width; }
    int height() const { return m_height; }
};
