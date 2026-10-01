#pragma once

#include "../gfx.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
  private:
    // the program ID
    unsigned int m_id;
  public:

    Shader(const char* vertex_path, const char* fragment_path) {
        // 1. retreiving vertex & fragment shaders' source code from their path

        std::string vertex_code;
        std::string fragment_code;
        std::ifstream vshader_file;
        std::ifstream fshader_file;
        // ensure ifstream objects can throw exceptions:
        vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // open files
            vshader_file.open(vertex_path);
            fshader_file.open(fragment_path);
            std::stringstream vshader_stream, fshader_stream;
            // read file's buffer contents into streams
            vshader_stream << vshader_file.rdbuf();
            fshader_stream << fshader_file.rdbuf();
            // close file handlers
            vshader_file.close();
            fshader_file.close();
            // convert stream into string
            vertex_code = vshader_stream.str();
            fragment_code = fshader_stream.str();

        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vshader_code = vertex_code.c_str();
        const char* fshader_code = fragment_code.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;
        int success;
        char info_log[512];

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vshader_code, NULL);
        glCompileShader(vertex);

        // check for compile err if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, info_log);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
        };

        // similiar for Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fshader_code, NULL);
        glCompileShader(fragment);

        // check for compile err
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, info_log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
        }

        // 3. creating shader program
        m_id = glCreateProgram();
        glAttachShader(m_id, vertex);
        glAttachShader(m_id, fragment);
        glLinkProgram(m_id);

        // check for linking err
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_id, 512, NULL, info_log);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        }

        // can delete shaders as they're already linked in program
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // the use function is straightforward
    void use() const { glUseProgram(m_id); }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const {
        glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const {
        glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const {
        glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
