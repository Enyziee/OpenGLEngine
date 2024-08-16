#pragma once

#include <glad/glad.h>
#include <iostream>

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const VertexArray& va, const IndexBuffer& ib);
    void drawArrays(const VertexArray& va);
    void clear();
};