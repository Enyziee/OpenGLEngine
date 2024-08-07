#pragma once

#include <iostream>

struct ShaderSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class ShaderProgram {
    uint32_t m_RendererID;

public:
    
    ShaderProgram(std::string path);

    void bind();
    void unbind();

    void setUniform4f(const char* uniform, float x, float y, float z, float a);
    void setUniform1i(const char* uniform, int value);

private:
    ShaderSource parseShaderFile(std::string shaderPath);
    uint32_t compileShader(std::string shaderSource, uint32_t type);
};
