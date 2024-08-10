#pragma once

#include <iostream>
#include "GLLog.h"

class IndexBuffer {
    uint32_t m_RendererID;
    uint32_t m_Count;

public:
    IndexBuffer(uint32_t* indexes, int count);
    ~IndexBuffer();

    int getCount() const;

    void bind() const;
    void unbind() const;
};