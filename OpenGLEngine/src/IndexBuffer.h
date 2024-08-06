#pragma once

#include <iostream>

class IndexBuffer {
    uint32_t m_BufferId;
public:
    IndexBuffer(float indexes[]);
    void bind();
    void unbind();
};