#include "app.h"
#include "GLFW/glfw3.h"

#include "render/primitives.h"

#include <iostream>
#include <memory>

App::App(const std::string_view title, const int width, const int height)
    : m_title { title }, m_width { width }, m_height { height } {
}

bool App::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    std::cout << "Initializing GLFW Window\n";

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, NULL);
    if (m_window == nullptr) {
        std::cerr << "Failed to create the window\n";
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Handle view port dimensions
    glViewport(0, 0, m_width, m_height);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });
    glfwSwapInterval(1);

    return true;
}

bool App::init() {
    bool glfw_success = initGLFW();
    if (!glfw_success) {
        return false;
    }

    m_input_handler = std::make_unique<InputHandler>(m_window);
    m_input_handler->init();

    m_camera = std::make_unique<Camera>((float)m_width / (float)m_height);
    m_shader =
        std::make_unique<Shader>("../assets/shaders/shader.vert", "../assets/shaders/shader.frag");

    m_input_handler->setMouseCallback(
        [this](const MouseMoveEvent& e) { m_camera->processMouseMovement(e.delta.x, e.delta.y); });

    // TODO: better way
    Texture tex {};
    tex.loadFile("../assets/textures/container.jpg");

    std::vector<Texture> textures;
    textures.push_back(std::move(tex));

    m_cube = std::make_unique<Mesh>(Primitives::makeCube(std::move(textures)));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    return true;
}

App::~App() {
    glfwTerminate();
}

bool App::run() {
    auto last_frame = (float)glfwGetTime();

    while (m_is_running && !glfwWindowShouldClose(m_window)) {
        auto current_frame = (float)glfwGetTime();
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glfwPollEvents();  // poll inputs first

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_camera->move(*m_input_handler, delta_time);

        glm::mat4 view = m_camera->getViewMatrix();
        glm::mat4 proj = m_camera->getProjectionMatrix();

        m_shader->use();
        m_shader->setMat4("u_pv", proj * view);

        m_cube->render(*m_shader);

        if (m_input_handler->isKeyPressed(KeyCode::Q)) {
            m_is_running = false;
            glfwSetWindowShouldClose(m_window, true);
        }

        m_input_handler->update();  // advance input step at end
        glfwSwapBuffers(m_window);
    }

    return true;
}
