#pragma once

#include "../gfx.h"
#include "buffer.h"

class VAO {
  private:
    GLuint m_id { 0 };

  public:
    VAO() = default;
    ~VAO() { destroy(); }

    VAO(VAO&& v) noexcept
        : m_id { v.m_id } {
        v.m_id = 0;
    }
    VAO& operator=(VAO&& v) noexcept {
        if (&v == this)
            return *this;

        destroy();
        m_id = v.m_id;
        v.m_id = 0;

        return *this;
    }

    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    void init() {
        if (m_id == 0) {
            glGenVertexArrays(1, &m_id);
        }
    }
    void bind() const {
        if (m_id != 0) {
            glBindVertexArray(m_id);
        }
    }
    void unbind() const { glBindVertexArray(0); }
    void destroy() {
        if (m_id != 0) {
            glDeleteVertexArrays(1, &m_id);
            m_id = 0;
        }
    }

    void addAttribute(Buffer& vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset) {
        this->bind();
        vbo.bind();
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(index);
    }
};
