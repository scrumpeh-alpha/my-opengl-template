#include "input_handler.h"

void InputHandler::init() {
    glfwSetWindowUserPointer(m_window.handle(), this);
    glfwSetCursorPosCallback(m_window.handle(), [](GLFWwindow* window, double xpos_in, double ypos_in) {
        auto input = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (!input || !input->m_capture_mouse) {
            return;
        }

        glm::vec2 pos = { static_cast<float>(xpos_in), static_cast<float>(ypos_in) };

        if (input->m_first_mouse) {
            input->m_last_pos = pos;
            input->m_first_mouse = false;
        }

        glm::vec2 delta = {
            pos.x - input->m_last_pos.x,
            input->m_last_pos.y - pos.y  // reversed since y-coordinates go from bottom to top.
        };

        input->m_last_pos = pos;
        input->m_mouse_callback(MouseMoveEvent { pos, delta });
    });

    glfwSetKeyCallback(m_window.handle(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto input = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (!input) {
            return;
        }
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            input->m_capture_mouse = !input->m_capture_mouse;
            if (input->m_capture_mouse) {
                input->m_first_mouse = true;
                glfwSetInputMode(input->m_window.handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else
                glfwSetInputMode(input->m_window.handle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        KeyCode code = keyFromGLFW(key);
        if (code == KeyCode::MaxKeycode)
            return;
        KeyState state = stateFromGLFW(action);
        if (state == KeyState::MaxKeystate)
            return;

        input->idxState(code) = state;
    });
}

void InputHandler::update() {
    m_prev_key_states = m_key_states;
    for (size_t i = 0; i < m_key_states.size(); ++i) {
        if (m_key_states[i] == KeyState::Press)
            m_key_states[i] = KeyState::Hold;
        if (m_key_states[i] == KeyState::Release)
            m_key_states[i] = KeyState::Inactive;
    }
}

