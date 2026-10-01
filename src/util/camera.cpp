#include "camera.h"
#include <glm/gtc/quaternion.hpp>

void Camera::move(const InputHandler& input, float dt) {
    glm::vec3 direction {};

    if (input.isKeyHeld(KeyCode::W)) direction += m_basis.front;
    if (input.isKeyHeld(KeyCode::S)) direction -= m_basis.front;
    if (input.isKeyHeld(KeyCode::A)) direction -= m_basis.right;
    if (input.isKeyHeld(KeyCode::D)) direction += m_basis.right;
    if (input.isKeyHeld(KeyCode::SPACE)) direction += m_basis.up;
    if (input.isKeyHeld(KeyCode::LCTRL)) direction -= m_basis.up;

    if (glm::dot(direction, direction) == 0.0f)
        return;

    direction = glm::normalize(direction);

    m_position += direction * m_speed * dt;
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    // update Front, Right and Up Vectors using the updated Euler angles
    updateBasis();
}

void Camera::updateBasis() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    // m_front = glm::normalize(front);
    m_basis.front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    m_basis.right = glm::normalize(glm::cross(m_basis.front, m_world_up));
    // normalize the vectors, because their length gets closer to 0 the
    // more you look up or down which results in slower movement.

    m_basis.up = glm::normalize(glm::cross(m_basis.right, m_basis.front));
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_basis.front, m_basis.up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(fov, aspect_ratio, z_near, z_far);
}
