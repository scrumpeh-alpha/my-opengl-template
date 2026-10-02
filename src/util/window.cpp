#include "window.h"
#include <iostream>

Window::Window(std::string_view title, const int width, const int height)
    : m_title { title }, m_width { width }, m_height { height } {}

bool Window::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    // TODO: change OpenGL version here
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    std::cout << "Initializing GLFW Window\n";

    m_handle = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, NULL);
    if (m_handle == nullptr) {
        std::cerr << "Failed to create the window\n";
        return false;
    }

    glfwMakeContextCurrent(m_handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(m_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Handle view port dimensions
    glViewport(0, 0, m_width, m_height);
    glfwSetFramebufferSizeCallback(m_handle, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });
    glfwSwapInterval(1);

    return true;
}

Window::~Window() {
    if (m_handle)
        glfwDestroyWindow(m_handle);
    glfwTerminate();
}
