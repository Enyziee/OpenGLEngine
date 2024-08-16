#include "Renderer.h"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib) {
    va.bind();
    ib.bind();
    
    glDrawElements(GL_TRIANGLES, ib.getCount() , GL_UNSIGNED_INT, (const void*)0);
}

void Renderer::drawArrays(const VertexArray& va) {

}

void Renderer::clear() {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}