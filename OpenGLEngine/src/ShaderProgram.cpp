#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(std::string vertPath, std::string fragPath) {
    std::string vertexSource = parseShaderFile(vertPath);
    std::string fragmentSource = parseShaderFile(fragPath);

    unsigned int  vertex = compileShader(vertexSource, GL_VERTEX_SHADER);
    unsigned int  frag = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    m_RendererID = glCreateProgram();
    std::cout << "Created shader program ID: " << m_RendererID << std::endl;


    glAttachShader(m_RendererID, vertex);
    glAttachShader(m_RendererID, frag);
    glLinkProgram(m_RendererID);

    int sucess;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &sucess);
    if (!sucess) {
        char infoLog[512];
        glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

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
    assert(uniformId);
    
    glUniform1i(uniformId, value);
}

void ShaderProgram::setUniform3fv(const char* uniform, const glm::vec3& value) const {
    int vLocation = glGetUniformLocation(this->m_RendererID, uniform);
    assert(vLocation != -1);

    glUniform3f(vLocation, value[0], value[1], value[2]);
}

void ShaderProgram::setUniform4fv(const char* uniform, const glm::vec4& value) const {
    int vLocation = glGetUniformLocation(this->m_RendererID, uniform);
    assert(vLocation != -1);
    
    glUniform4fv(vLocation, 4, glm::value_ptr(value));
}

void ShaderProgram::setUniformMatrix4fv(const char* uniform, const glm::mat4& value) const {
    int uniformId = glGetUniformLocation(this->m_RendererID, uniform);
    assert(uniformId != -1);
    glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(value));
}

std::string ShaderProgram::parseShaderFile(std::string shaderPath) {
    std::ifstream sourceFile(shaderPath);

    if (!sourceFile) {
        std::cout << "Cannot open the file" << std::endl;
    }

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
        std::cerr << "Shader compilation failed\n" << infoLog << std::endl;
    }

    if (type == GL_VERTEX_SHADER) {
        std::cout << "Created Vertex Shader ID: " << shader << std::endl;
    } else if (type == GL_FRAGMENT_SHADER) {
        std::cout << "Created Fragment Shader ID: " << shader << std::endl;
    }
    else {
        std::cout << "Created Unknown Shader ID: " << shader << std::endl;
    }

    return shader;
}