#include "app.h"
#include "GLFW/glfw3.h"

#include "render/primitives.h"

#include <memory>

App::App(const std::string_view title, const int width, const int height)
    : m_window { title, width, height } {
}

bool App::init() {
    if (!m_window.init()) {
        return false;
    }

    m_input_handler = std::make_unique<InputHandler>(m_window);
    m_input_handler->init();

    m_camera = std::make_unique<Camera>((float)m_window.width() / (float)m_window.height());

    // TODO: replace hardcoded paths
    m_shader =
        std::make_unique<Shader>("../assets/shaders/shader.vert", "../assets/shaders/shader.frag");

    m_input_handler->setMouseCallback(
        [this](const MouseMoveEvent& e) { m_camera->processMouseMovement(e.delta.x, e.delta.y); });

    // TODO: paths + texture management with cache
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

bool App::run() {
    auto last_frame = (float)glfwGetTime();

    while (m_is_running && !m_window.shouldClose()) {
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
            m_window.close();
        }

        m_input_handler->update();  // advance input step at end
        m_window.swapBuffers();
    }

    return true;
}
