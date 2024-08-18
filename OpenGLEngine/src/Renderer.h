#pragma once

#include <glad/glad.h>
#include <iostream>

#include "ShaderProgram.h"
#include "Buffer.h"
#include "VertexArray.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const VertexBuffer& vb, const IndexBuffer& ib);
    void drawArrays(VertexArray& vb);

    void clear();
};