#pragma once

#include "../gfx.h"
#include <iostream>

class Buffer {
  public:
    bool dynamic { false };

  private:
    GLuint m_id { 0 };
    GLenum m_type { 0 };

  public:
    explicit Buffer(GLenum type)
        : m_type { type } {}

    ~Buffer() { destroy(); }

    Buffer(Buffer&& v) noexcept
        : m_id { v.m_id }, m_type { v.m_type } {
        v.m_id = 0;
        // v.m_type = 0;
    }

    Buffer& operator=(Buffer&& v) noexcept {
        if (&v == this)
            return *this;

        this->destroy();
        m_id = v.m_id;
        m_type = v.m_type;
        v.m_id = 0;
        // v.m_type = 0;

        return *this;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    void init() {
        if (m_id == 0) {
            glGenBuffers(1, &m_id);
        }
    }

    void bind() const {
        if (m_id != 0 && m_type != 0) {
            glBindBuffer(m_type, m_id);
        }
    }

    void unbind() const {
        if (m_type != 0) {
            glBindBuffer(m_type, 0);
        }
    }

    void destroy() {
        if (m_id != 0) {
            glDeleteBuffers(1, &m_id);
            m_id = 0;
            // m_type = 0;
        }
    }

    void setBuffer(void* data, size_t size_in_bytes) {
        if (m_id != 0 && m_type != 0) {
            bind();
            GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
            glBufferData(m_type, size_in_bytes, data, usage);  // send VBO data
        }
    }
};
