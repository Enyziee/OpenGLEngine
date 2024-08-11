#pragma once

#include <glad/glad.h>
#include <iostream>

class IndexBuffer {
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBuffer(uint32_t* indexes, int count);
    ~IndexBuffer();

    int getCount() const;

    void bind() const;
    void unbind() const;
};