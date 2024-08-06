#pragma once

#include <iostream>

struct ShaderSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class ShaderProgram {
    uint32_t m_ProgramId;

public:
    ShaderProgram(std::string path);

    void bind();
    void unbind();

    void setUniform4f(float x, float y, float z, float a, const char* uniform);

private:
    ShaderSource parseShaderFile(std::string shaderPath);
    uint32_t compileShader(std::string shaderSource, uint32_t type);
};
