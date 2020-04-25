#include <GL/glew.h>
#include <iostream>
#include "glutils.hpp"

void GLClearError(){
    while (glGetError());
}

bool GLCheckError(const char* func, const char* file, int line){
    while (GLenum error = glGetError())
    {
        std::cout << "[ERROR 0x" << std::hex << error << "] " << func << ":" << file << ":" << std::dec << line <<std::endl;
        return false;
    }
    return true;
}

VertexBuffer::VertexBuffer(const void* data, const uint size){
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer(){
    GlCall(glDeleteBuffers(1, &m_renderer_id));
}

void VertexBuffer::Bind() const{
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
}

void VertexBuffer::Unbind() const {
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

IndexBuffer::IndexBuffer(const uint* data, uint count):
    m_count(count)
{
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer(){
    GlCall(glDeleteBuffers(1, &m_renderer_id));
}

void IndexBuffer::Bind() const {
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
}

void IndexBuffer::Unbind() const {
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}