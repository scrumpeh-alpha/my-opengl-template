#pragma once
#include "../graphics/classes/buffer.h"
#include "../graphics/classes/shader.h"
#include "../graphics/classes/texture.h"
#include "../graphics/classes/vao.h"
#include "../graphics/classes/vertex.h"

class Mesh {
  private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<Texture> m_textures;

    Buffer m_vbo { GL_ARRAY_BUFFER };
    Buffer m_ebo { GL_ELEMENT_ARRAY_BUFFER };
    VAO m_vao {};

    glm::mat4 m_model { 1.0f };

  public:
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices,
         std::vector<Texture> textures = {});

    // make sure GL objects creation happens on main thread
    void upload();

    void render(const Shader& shader) const;

    const glm::mat4& model() { return m_model; }
    void setModel(const glm::mat4& model) { m_model = model; }
};
