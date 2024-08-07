#pragma once

#include <iostream>

class IndexBuffer {
    uint32_t m_RendererID;
    uint32_t m_Count;

public:
    IndexBuffer(unsigned int* indexes, int count);
    ~IndexBuffer();

    void bind();
    void unbind();
};