#pragma once

#include "../graphics/gfx.h"
#include <string>

class Window {
  private:
    GLFWwindow* m_handle { nullptr };
    std::string m_title;
    int m_width;
    int m_height;

  public:
    Window(std::string_view title, const int width, const int height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool init();

    bool shouldClose() const { return glfwWindowShouldClose(m_handle); }
    void close() { glfwSetWindowShouldClose(m_handle, true); }

    void swapBuffers() const { glfwSwapBuffers(m_handle); }

    GLFWwindow* handle() const { return m_handle; }
    int width() const { return m_width; }
    int height() const { return m_height; }
};
