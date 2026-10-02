#pragma once
#include "graphics/classes/shader.h"
#include "graphics/gfx.h"
#include "render/mesh.h"
#include "util/input_handler.h"
#include "util/camera.h"
#include "util/window.h"

#include <memory>
#include <string>

class App {
  private:
    Window m_window;
    std::unique_ptr<InputHandler> m_input_handler;

    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Shader> m_shader;

    std::unique_ptr<Mesh> m_cube;

    bool m_is_running { true };

  public:
    App(std::string_view title, int width, int height);

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    bool init();
    bool run(); // option to have return code later
};

