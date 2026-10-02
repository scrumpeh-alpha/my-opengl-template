#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices,
           std::vector<Texture> textures)
    : m_vertices { std::move(vertices) },
      m_indices { std::move(indices) },
      m_textures { std::move(textures) } {
    upload();
}

void Mesh::upload() {
    if (m_vertices.empty()) {
        return;
    }

    m_vao.init();
    m_vbo.init();
    m_ebo.init();

    m_vao.bind();

    m_vbo.bind();
    m_vbo.setBuffer(m_vertices.data(), m_vertices.size() * sizeof(Vertex));

    m_ebo.bind();
    m_ebo.setBuffer(m_indices.data(), m_indices.size() * sizeof(uint32_t));

    m_vao.addAttribute(m_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
    m_vao.addAttribute(m_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
    m_vao.addAttribute(m_vbo, 2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, uv));

    m_vao.unbind();
}

void Mesh::render(const Shader& shader) const {
    shader.use();
    shader.setMat4("u_model", m_model);

    for (size_t i = 0; i < m_textures.size(); i++) {
        std::string name = "u_texture" + std::to_string(i);
        shader.setInt(name, i);

        m_textures[i].bind(i);  // bind with texture unit
    }

    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.unbind();

    // needed?
    // glActiveTexture(GL_TEXTURE0);
}
