
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

        WindowHandler->poll_events();
        WindowHandler->render_imgui();
        WindowHandler->swap_buffers();

    }

    return 0;
}


