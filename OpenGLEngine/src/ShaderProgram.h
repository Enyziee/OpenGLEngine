#pragma once

#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

struct ShaderSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class ShaderProgram {
    unsigned int m_RendererID;

public:
    
    ShaderProgram(std::string vertPath, std::string fragPath);
    ~ShaderProgram();

    void bind() const;
    void unbind() const;

    void setUniform1i(const char* uniform, int value) const;

    void setUniform3fv(const char* uniform, const glm::vec3& value) const;
    void setUniform4fv(const char* uniform, const glm::vec4& value) const;
    
    void setUniformMatrix4fv(const char* uniform, const glm::mat4& value) const;

private:
    std::string parseShaderFile(std::string shaderPath);
    unsigned int compileShader(std::string shaderSource, unsigned int type);
};
