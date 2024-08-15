#pragma once

#include <glad/glad.h>
#include <iostream>

class IndexBuffer {
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBuffer();
    IndexBuffer(int* indexes, size_t count);
    ~IndexBuffer();

    void loadData(int* indexes, size_t count);

    int getCount() const;

    void bind() const;
    void unbind() const;
private:
    
};