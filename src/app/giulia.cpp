
#include <iostream>
#include "renderer.hpp"
#include "window_handler.hpp"
// #include "glutils.hpp"
#include "log.hpp"

int main() {

    Window::Handler *WindowHandler;
    Renderer::Handler *Renderer;
    try {
        WindowHandler = new Window::Handler();
        Renderer = new Renderer::Handler();
    }
    catch (Window::InitException& e)
    {
        std::cout << e.what() << '\n';
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << ": Unknown Error! Exiting!\n";
        return -1;
    }

    print_hardware_info();

    while (WindowHandler->user_has_not_exited()) {
        compute_mode_t mode = WindowHandler->get_selected_compute_mode();
        // compute_target_t target = MANDELBROT;

        Renderer->Clear();

        complex_number aux = WindowHandler->get_left_position();
        complex_number aux2 = WindowHandler->get_right_position();
        complex_number aux3 = WindowHandler->get_cursor_position();

        Renderer->Draw(LEFT, mode, MANDELBROT, aux, aux3, WindowHandler->get_iter_number(), WindowHandler->get_selected_precision_mode());
        Renderer->Draw(RIGHT, mode, JULIA, aux2, aux3, WindowHandler->get_iter_number(), WindowHandler->get_selected_precision_mode());
        Renderer->DrawCursor(aux, aux3);

        Renderer->DrawLogo(mode);

        WindowHandler->poll_events();
        WindowHandler->render_imgui();
        WindowHandler->swap_buffers();


        // LOG_INFO("X: " << aux.real << " Y: " << aux.imaginary);

    }

    return 0;
}


