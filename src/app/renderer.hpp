#ifndef RENDERER_H
#define RENDERER_H

#include "glutils.hpp"

namespace Renderer{


class Handler{
    public:
        // void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        // void Clear() const;

        void DrawLogo(compute_mode_t mode);

        Handler();
        ~Handler();
    private:
        VertexBufferLayout layout;
        VertexBufferLayout texture_layout;

        // VertexArray left_half_va;
        // VertexBuffer left_half_vb;

        VertexArray     *va_texture;
        VertexBuffer    *vb_texture;
        IndexBuffer     *ib_texture;

};
}

#endif
