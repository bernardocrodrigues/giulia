
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

        // Renderer->Draw(LEFT, mode, MANDELBROT);
        // Renderer->Draw(RIGHT, mode, MANDELBROT);
        Renderer->Draw(FULL, mode, MANDELBROT);

        Renderer->DrawLogo(mode);

        WindowHandler->poll_events();
        WindowHandler->render_imgui();
        WindowHandler->swap_buffers();

    }

    return 0;
}


