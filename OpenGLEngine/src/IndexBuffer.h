#pragma once

#include "glcommon.h"

class IndexBuffer {
    uint32_t m_RendererID;
    uint32_t m_Count;

public:
    IndexBuffer(uint32_t* indexes, int count);
    ~IndexBuffer();

    void bind();
    void unbind();
};