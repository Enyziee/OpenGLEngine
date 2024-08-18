#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Log.h"

ShaderProgram::ShaderProgram(std::string filePath) : m_RendererID(0) {
    auto vertPath = std::string(filePath + ".vert");
    auto fragPath = std::string(filePath + ".frag");

    std::string vertexSource = parseShaderFile(vertPath);
    std::string fragmentSource = parseShaderFile(fragPath);

    if (vertexSource.size() == 0) {
        LOGGER_ERROR("Cannot read file shader: {0}", vertPath);
        return;
    }

    unsigned int vertex = compileShader(vertexSource, GL_VERTEX_SHADER);
    unsigned int frag = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    if (!vertex || !frag) {
        m_RendererID = 0;
        return;
    }

    unsigned int program = glCreateProgram();

    glAttachShader(program, vertex);
    glAttachShader(program, frag);
    
    glLinkProgram(program);

    int sucess;
    glGetProgramiv(program, GL_LINK_STATUS, &sucess);
    if (!sucess) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        LOGGER_ERROR("Shader linking failed: {0}", infoLog);
    }

    m_RendererID = program;

    glDetachShader(program, vertex);
    glDetachShader(program, frag);

    glDeleteShader(vertex);
    glDeleteShader(frag);
    
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_RendererID);
}

void ShaderProgram::bind() const {
    glUseProgram(m_RendererID);
}

void ShaderProgram::unbind() const {
    glUseProgram(0);
}

void ShaderProgram::setUniform1i(const char* uniform, int value) const {
    int uniformId = glGetUniformLocation(this->m_RendererID, uniform);
    glUniform1i(uniformId, value);
}

void ShaderProgram::setUniform3fv(const char* uniform, const glm::vec3& value) const {
    int vLocation = glGetUniformLocation(this->m_RendererID, uniform);
    glUniform3f(vLocation, value[0], value[1], value[2]);
}

void ShaderProgram::setUniform4fv(const char* uniform, const glm::vec4& value) const {
    int vLocation = glGetUniformLocation(this->m_RendererID, uniform);
    glUniform4fv(vLocation, 4, glm::value_ptr(value));
}

void ShaderProgram::setUniformMatrix4fv(const char* uniform, const glm::mat4& value) const {
    int uniformId = glGetUniformLocation(this->m_RendererID, uniform);
    glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(value));
}

std::string ShaderProgram::parseShaderFile(std::string shaderPath) {
    std::ifstream sourceFile(shaderPath);

    std::string line;
    std::stringstream ss;

    while (getline(sourceFile, line)) {
        ss << line << '\n';
    }

    return ss.str();
}

unsigned int ShaderProgram::compileShader(std::string shaderSource, unsigned int type) {
    unsigned int shader = glCreateShader(type);
    const char* source = shaderSource.c_str();
    int sucess;

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
    if (!sucess) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOGGER_ERROR("Shader compilation failed {0}", infoLog);
    }

    return shader;
}