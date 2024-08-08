#include "ShaderProgram.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

ShaderProgram::ShaderProgram(std::string path) {
    ShaderSource shaders = parseShaderFile(path);

    m_RendererID = glCreateProgram();
    uint32_t vertex = compileShader(shaders.VertexSource, GL_VERTEX_SHADER);
    uint32_t frag = compileShader(shaders.FragmentSource, GL_FRAGMENT_SHADER);

    GLCall(glAttachShader(m_RendererID, vertex));
    GLCall(glAttachShader(m_RendererID, frag));
    GLCall(glLinkProgram(m_RendererID));

    int sucess;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &sucess);
    if (!sucess) {
        char infoLog[512];
        glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void ShaderProgram::bind() {
    glUseProgram(m_RendererID);
}

void ShaderProgram::unbind() {
    glUseProgram(0);
}

void ShaderProgram::setUniform4f(const char* uniform, float x, float y, float z, float a) {
    int vLocation = glGetUniformLocation(this->m_RendererID, uniform);
    glUniform4f(vLocation, x, y, z, a);
}

void ShaderProgram::setUniform1i(const char* uniform, int value) {
    int uniformId = glGetUniformLocation(this->m_RendererID, uniform);
    glUniform1i(uniformId, value);
}

void ShaderProgram::setUniformMatrix4fv(const char* uniform, glm::f32* value) {
    int uniformId = glGetUniformLocation(this->m_RendererID, uniform);
    GLCall(glUniformMatrix4fv(uniformId, 1, GL_FALSE, value));
}

ShaderSource ShaderProgram::parseShaderFile(std::string shaderPath) {
    std::ifstream sourceFile(shaderPath);

    if (!sourceFile) {
        std::cout << "Cannot open the file" << std::endl;
    }

    enum ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(sourceFile, line)) {

        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }

        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

uint32_t ShaderProgram::compileShader(std::string shaderSource, uint32_t type) {
    uint32_t shader = glCreateShader(type);
    const char* source = shaderSource.c_str();
    int sucess;

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
    if (!sucess) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation failed\n" << infoLog << std::endl;
    }

    return shader;
}