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
    
    ShaderProgram(std::string path);
    ~ShaderProgram();

    void bind() const;
    void unbind() const;

    void setUniform4f(const char* uniform, float x, float y, float z, float a) const;
    void setUniform1i(const char* uniform, int value) const;
    void setUniformMatrix4fv(const char* uniform, glm::f32* value) const;

private:
    ShaderSource parseShaderFile(std::string shaderPath);
    unsigned int compileShader(std::string shaderSource, unsigned int type);
};
