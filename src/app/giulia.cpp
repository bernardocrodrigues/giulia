
#include <iostream>
#include "renderer.hpp"
#include "window_handler.hpp"
#include "log.hpp"

int main() {

    Win::Handler *WindowHandler;
    Renderer::Handler *Renderer;
    try {
        WindowHandler = new Win::Handler();
        Renderer = new Renderer::Handler(
            (cl_context_properties)WindowHandler->get_glx_context(),
            (cl_context_properties)WindowHandler->get_x11_display());
        // Renderer = new Renderer::Handler();
    }
    catch (Win::InitException& e)
    {
        std::cout << e.what() << '\n';
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << ": Unknown Error! Exiting!\n";
        return -1;
    }

    while (WindowHandler->user_has_not_exited()) {
        compute_mode_t mode = WindowHandler->get_selected_compute_mode();

        Renderer->Clear();

        complex_number aux = WindowHandler->get_left_position();
        complex_number aux2 = WindowHandler->get_right_position();
        complex_number aux3 = WindowHandler->get_cursor_position();

        switch (WindowHandler->get_fullscreen_mode()) {
          case fullscreen_mode_t::NONE:
            Renderer->Draw(window_region_t::LEFT, mode,
                           compute_target_t::MANDELBROT, aux, aux3,
                           WindowHandler->get_iter_number(),
                           WindowHandler->get_selected_precision_mode(),
                           WindowHandler->get_x_range_on_left(),
                           WindowHandler->get_color_preset_number(),
                           WindowHandler->get_exponent());
            Renderer->Draw(window_region_t::RIGHT, mode,
                           compute_target_t::JULIA, aux2, aux3,
                           WindowHandler->get_iter_number(),
                           WindowHandler->get_selected_precision_mode(),
                           WindowHandler->get_x_range_on_right(),
                           WindowHandler->get_color_preset_number(),
                           WindowHandler->get_exponent());
            Renderer->DrawCursor(aux, aux3,
                                 WindowHandler->get_x_range_on_left());
            break;
          case fullscreen_mode_t::MANDELBROT:
            Renderer->Draw(window_region_t::FULL, mode,
                           compute_target_t::MANDELBROT, aux, aux3,
                           WindowHandler->get_iter_number(),
                           WindowHandler->get_selected_precision_mode(),
                           WindowHandler->get_x_range_on_left(),
                           WindowHandler->get_color_preset_number(),
                           WindowHandler->get_exponent());
            break;
          case fullscreen_mode_t::JULIA:
            Renderer->Draw(window_region_t::FULL, mode, compute_target_t::JULIA,
                           aux2, aux3, WindowHandler->get_iter_number(),
                           WindowHandler->get_selected_precision_mode(),
                           WindowHandler->get_x_range_on_right(),
                           WindowHandler->get_color_preset_number(),
                           WindowHandler->get_exponent());
            break;
          default:
            break;
        }
        Renderer->DrawLogo(mode);
        WindowHandler->render_imgui();
        WindowHandler->swap_buffers();
        WindowHandler->poll_events();
    }
    return 0;
}


