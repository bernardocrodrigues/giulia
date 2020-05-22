

#include <iostream>
#include "window_handler.hpp"
#include "glutils.hpp"
#include "log.hpp"


int main() {

    Window::Handler *WindowHandler;

    try {
        WindowHandler = new Window::Handler();
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

        LOG_INFO( "Mode: " << (WindowHandler->get_selected_compute_mode() == Window::OPENGL_MODE ? "OpenGL" : "OpenCL"));
    }

    return 0;
}


