cmake_minimum_required(VERSION 3.20)
project(
    OpenGLApp
    VERSION 1.0
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup default build output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)

# ==============================================================================
# TARGET DEFINITION
# ==============================================================================

add_executable(${PROJECT_NAME}
    src/graphics/gfx.h
    src/graphics/classes/buffer.h
    src/graphics/classes/shader.h
    src/graphics/classes/texture.cpp
    src/graphics/classes/texture.h
    src/graphics/classes/vao.h
    src/graphics/classes/vertex.h  

    src/render/mesh.cpp
    src/render/mesh.h
    src/render/primitives.cpp
    src/render/primitives.h

    src/util/camera.cpp
    src/util/camera.h
    src/util/input_handler.cpp
    src/util/input_handler.h

    src/app.cpp
    src/app.h
    src/main.cpp
)

# ==============================================================================
# COMPILER OPTIMIZATIONS (Release)
# ==============================================================================

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/O2;/Ob2;/Oi;/Ot;/arch:AVX2>
        $<$<CONFIG:Release>:/Zi>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/DEBUG:FULL;/OPT:REF;/OPT:ICF>
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-O3;-ffast-math>
    )
endif()

# ==============================================================================
# EXTERNAL DEPENDENCIES
# ==============================================================================

# --------------- GLFW ---------------

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# --------------- GLAD ---------------

add_library(glad STATIC lib/glad.c)
target_include_directories(glad PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

# -------------- OpenGL --------------
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

# --------------- GLM ----------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 2d4c4b4dd31fde06cfffad7915c2b3006402322f
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

# ------------ STB Image -------------
add_library(stb_image STATIC lib/stb_image.cpp)
target_include_directories(stb_image PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE stb_image)

# -------------- ImGUI ---------------
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG "v1.92.1"
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glfw_SOURCE_DIR}/include
)
target_link_libraries(imgui PRIVATE glfw glad)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# ==============================================================================
# POST-BUILD & IDE CONFIGURATION
# ==============================================================================

# Copy assets directory to executable output folder
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Set startup target for Visual Studio
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${PROJECT_NAME})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
