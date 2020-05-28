
#include "renderer.hpp"
#include "data_def.hpp"

// void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

//     shader.Bind();
//     va.Bind();
//     ib.Bind();
//     GlCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
// }

// void Renderer::Clear() const {
//     GlCall(glClear(GL_COLOR_BUFFER_BIT));
// }

Renderer::Handler::Handler() {

    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    layout.Push<float>(2);
    texture_layout.Push<float>(2);
    texture_layout.Push<float>(2);

    va_texture = new VertexArray();
    vb_texture = new VertexBuffer(logo_positions, 16 * sizeof(float));
    va_texture->AddBuffer(*vb_texture, texture_layout);
    ib_texture = new IndexBuffer(indices, 6);

}

Renderer::Handler::~Handler(){

}
