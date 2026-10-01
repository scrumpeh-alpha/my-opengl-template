#pragma once
#include "../graphics/classes/vao.h"
#include "../graphics/classes/buffer.h"
#include "../graphics/classes/shader.h"
#include "glm/glm.hpp"
#include <array>

namespace CubeData {

struct Vertex {
    glm::vec3 pos {};

    // temporary
    constexpr Vertex(float x, float y, float z) : pos(x, y, z) {}
};

constexpr std::array<Vertex, 8> VERTICES {
    Vertex { -1.0f,  1.0f,  1.0f }, // Top-Front-Left
    Vertex { -1.0f, -1.0f,  1.0f }, // Bottom-Front-Left
    Vertex {  1.0f,  1.0f,  1.0f }, // Top-Front-Right
    Vertex {  1.0f, -1.0f,  1.0f }, // Bottom-Front-Right
    Vertex { -1.0f,  1.0f, -1.0f }, // Top-Back-Left
    Vertex { -1.0f, -1.0f, -1.0f }, // Bottom-Back-Left
    Vertex {  1.0f,  1.0f, -1.0f }, // Top-Back-Right
    Vertex {  1.0f, -1.0f, -1.0f }  // Bottom-Back-Right
};

constexpr std::array<uint32_t, 36> INDICES {
    0, 2, 3, 0, 3, 1, 2, 6, 7, 2, 7, 3, 6, 4, 5, 6, 5, 7,
    4, 0, 1, 4, 1, 5, 0, 4, 6, 0, 6, 2, 1, 5, 7, 1, 7, 3,
};

};  // namespace CubeData

class Cube {
  private:
    std::array<CubeData::Vertex, 8> m_vertices { CubeData::VERTICES };
    std::array<uint32_t, 36> m_indices { CubeData::INDICES };

    Buffer m_vbo { GL_ARRAY_BUFFER };
    Buffer m_ebo { GL_ELEMENT_ARRAY_BUFFER };
    VAO m_vao {};

    glm::mat4 m_model { 1.0f };

  public:
    Cube() {
        upload(); // move if need to create objects on separate threads
    }
    ~Cube() = default;

    void upload() {
        if (m_vertices.empty()) {
            return;
        }

        // make sure GL objects creation happens on main thread
        m_vao.init();
        m_vbo.init();
        m_ebo.init();

        m_vao.bind();

        m_vbo.bind();
        m_vbo.setBuffer(m_vertices.data(), m_vertices.size() * sizeof(CubeData::Vertex));

        m_vao.addAttribute(m_vbo, 0, 3, GL_FLOAT, sizeof(CubeData::Vertex), 0);
        // m_vao.addAttribute(m_vbo, 1, 3, GL_FLOAT, sizeof(float), 3 * sizeof(float));
        // m_vao.addAttribute(m_vbo, 2, 2, GL_FLOAT, sizeof(float), 6 * sizeof(float));

        m_ebo.bind();
        m_ebo.setBuffer(m_indices.data(), m_indices.size() * sizeof(uint32_t));
        
        m_vao.unbind();
    }

    void render(const Shader& shader) const {
        shader.use();
        m_vao.bind();
        shader.setMat4("u_model", m_model);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        m_vao.unbind();
    }
};
