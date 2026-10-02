#pragma once
#include "glm/glm.hpp"
#include "input_handler.h"

struct Basis {
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
};

class Camera {
    // Constants
  public:
    static constexpr float PITCH = 0.0f;
    static constexpr float YAW = 0.0f;
    static constexpr float FOV = 45.0f;
    static constexpr float SENSITIVITY = 0.1f;
    static constexpr float PITCH_LIMIT = 89.0f;
    static constexpr float NEAR_DIST = 0.1f;
    static constexpr float FAR_DIST = 1000.0f;
    static constexpr float SPEED = 3.0f;
    static constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    static constexpr glm::vec3 POSITION = glm::vec3(0.0f, 1.0f, 3.0f);

    float fov { FOV };
    float aspect_ratio;

    float z_near { NEAR_DIST }, z_far { FAR_DIST };

  private:
    float m_yaw { YAW }, m_pitch { PITCH };
    float m_sensitivity { SENSITIVITY };

    glm::vec3 m_position { POSITION };
    glm::vec3 m_speed { SPEED };
    glm::vec3 m_world_up { WORLD_UP };

    Basis m_basis {};

  public:
    explicit Camera(float aspect_ratio, const glm::vec3& position = POSITION,
                    const glm::vec3& world_up = WORLD_UP)
        : aspect_ratio { aspect_ratio },
          m_position { position },
          m_world_up { world_up } {}

    void move(const InputHandler& input, float dt);
    void processMouseMovement(float xoffset, float yoffset);
    void updateBasis();

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    const glm::vec3& position() const { return m_position; }
    void setPosition(const glm::vec3& new_pos) { m_position = new_pos; }

    const glm::vec3& worldUp() const { return m_world_up; }
};
