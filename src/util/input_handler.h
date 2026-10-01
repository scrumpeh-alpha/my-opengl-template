#pragma once
#include "../graphics/gfx.h"
#include "glm/glm.hpp"

#include <array>
#include <functional>

enum class KeyCode : uint8_t {
    // clang-format off
    ESC,
    SPACE, LCTRL,
    W, A, S, D,

    Q,

    UP, DOWN,

    MaxKeycode
    // clang-format on
};

enum class KeyState {
    Inactive = 0,
    Press = 1,
    Release = 2,
    Hold = 3,

    MaxKeystate
};

struct MouseMoveEvent {
    glm::vec2 pos;
    glm::vec2 delta;
};

using MouseCallback = std::function<void(const MouseMoveEvent&)>;

class InputHandler {
  private:
    std::array<KeyState, static_cast<size_t>(KeyCode::MaxKeycode)> m_key_states {};
    std::array<KeyState, static_cast<size_t>(KeyCode::MaxKeycode)> m_prev_key_states {};

  private:
    GLFWwindow* m_window { nullptr };

    glm::vec2 m_last_pos {};
    bool m_first_mouse { true };
    bool m_capture_mouse { true };

    MouseCallback m_mouse_callback;

  public:
    explicit InputHandler(GLFWwindow* window)
        : m_window { window } {}

    void init();
    void update();  // run before every frame?

    void setMouseCallback(const MouseCallback& callback) { m_mouse_callback = callback; }

    bool isKeyPressed(KeyCode key) const { return idxState(key) == KeyState::Press; }
    bool isKeyHeld(KeyCode key) const { return idxState(key) == KeyState::Hold; }

    const KeyState& idxState(KeyCode key) const { return m_key_states[static_cast<size_t>(key)]; }
    KeyState& idxState(KeyCode key) { return m_key_states[static_cast<size_t>(key)]; }

    const KeyState& idxPrevState(KeyCode key) const {
        return m_prev_key_states[static_cast<size_t>(key)];
    }
    KeyState& idxPrevState(KeyCode key) { return m_prev_key_states[static_cast<size_t>(key)]; }

    static KeyCode keyFromGLFW(int key) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            return KeyCode::ESC;
        case GLFW_KEY_SPACE:
            return KeyCode::SPACE;
        case GLFW_KEY_LEFT_CONTROL:
            return KeyCode::LCTRL;

        case GLFW_KEY_W:
            return KeyCode::W;
        case GLFW_KEY_A:
            return KeyCode::A;
        case GLFW_KEY_S:
            return KeyCode::S;
        case GLFW_KEY_D:
            return KeyCode::D;

        case GLFW_KEY_Q:
            return KeyCode::Q;

        case GLFW_KEY_UP:
            return KeyCode::UP;
        case GLFW_KEY_DOWN:
            return KeyCode::DOWN;

        default:
            return KeyCode::MaxKeycode;
        }
    }

    static KeyState stateFromGLFW(int action) {
        switch (action) {
        case GLFW_PRESS:
            // case GLFW_REPEAT: <-- don't need for now
            return KeyState::Press;
        case GLFW_RELEASE:
            return KeyState::Release;
        default:
            return KeyState::MaxKeystate;
        }
    }
};
