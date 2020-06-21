#ifndef RENDERER_H
#define RENDERER_H

#include "glutils.hpp"
#include "data_def.hpp"

namespace Renderer{

class Pimpl;

class Handler{
    public:

        void Clear() const;
        void DrawLogo(compute_mode_t& mode);
        void DrawCursor(complex_number origin, complex_number position);
        void Draw(window_region_t region, compute_mode_t& mode, compute_target_t target, complex_number position, complex_number c);

        Handler();
        ~Handler();
    private:
        Pimpl* pimpl_;
};
}

#endif
