
#include <iostream>
#include "data_def.hpp"
#include "window_handler.hpp"
#include "log.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Window {

class Pimpl {
    public:
        bool imgui_focus;
        GLFWwindow* window;
        compute_mode_t compute_mode;

        Pimpl();
        ~Pimpl();
};

Pimpl::Pimpl() : imgui_focus(false), compute_mode(OPENGL_MODE) {}

static void glfw_error_callback(int error, const char* desc) {
    (void) error;
    (void) desc;
    fputs(desc,stderr);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    (void) mods;
    double xpos, ypos;
    Pimpl *pimpl_ = reinterpret_cast<Pimpl*>(glfwGetWindowUserPointer(window));

    glfwGetCursorPos(window, &xpos, &ypos);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){

        if(pimpl_->imgui_focus){
            LOG_INFO_WITH_CONTEXT("Mouse press at " << xpos << "x" << (HEIGHT - ypos) << "[Inside ImGUI Window]");
        } else {
            LOG_INFO_WITH_CONTEXT("Mouse press at " << xpos << "x" << (HEIGHT - ypos));
        }

        // if (!hover){
            // cursor_screen = {xpos, HEIGHT - ypos};
        // }
        // mouse = MOUSE_JUST_CLICKED;
    }else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        LOG_INFO_WITH_CONTEXT("Mouse release at " << xpos << "x" << (HEIGHT - ypos));

        // mouse = MOUSE_RELEASSED;
        // draw_request = 2;
    }
}

Handler::Handler():
    pimpl_(new Pimpl()){

    if (!glfwInit())
        throw InitException("Error on glfwInit");

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback(glfw_error_callback);

    pimpl_->window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);

    if (!pimpl_->window) {
        glfwTerminate();
        throw InitException(" Window was not created sucessfully");
    }

    glfwMakeContextCurrent(pimpl_->window);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(pimpl_->window, reinterpret_cast<void*>(pimpl_));
    glfwSetMouseButtonCallback(pimpl_->window, mouse_button_callback);

    if (glewInit() != GLEW_OK){
        throw InitException("GLEW was not initialized sucessfully");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(pimpl_->window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

}

Handler::~Handler() {
    glfwDestroyWindow(pimpl_->window);
    glfwTerminate();
}

bool Handler::user_has_not_exited() { return !glfwWindowShouldClose(pimpl_->window); }
void Handler::poll_events() { glfwPollEvents(); }
// void Handler::poll_events() { glfwWaitEvents(); }
void Handler::swap_buffers() { glfwSwapBuffers(pimpl_->window); }

compute_mode_t Handler::get_selected_compute_mode() { return pimpl_->compute_mode; }


void Handler::render_imgui() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        static bool opengl = true;
        static bool opengl_old = true;
        static bool opencl = false;
        static bool opencl_old = false;

        ImGui::Begin("Settings");                          // Create a window called "Hello, world!" and append into it.

        // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Another Window", &show_another_window);

        // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        pimpl_->imgui_focus = ImGui::IsWindowHovered();

        // LOG_INFO(pimpl_->imgui_focus);
        // pimpl_->imgui_focus = ImGui::IsWindowFocused();
        // ImGuiIO *io = &ImGui::GetIO();

        // io.WantCaptureMous = false;
        // hover = io->MouseDown[0];

        ImGui::Text("Compute Mode: ");
        ImGui::SameLine();
        ImGui::Checkbox("OpenGL", &opengl);
        ImGui::SameLine();
        ImGui::Checkbox("OpenCL", &opencl);

        if (opengl != opengl_old) {
            opengl_old = opengl;
            if (opengl) {
                pimpl_->compute_mode = OPENGL_MODE;
                LOG_INFO_WITH_CONTEXT("OPENGL_MODE");
                if (opencl)
                    opencl = false;
            } else {
                if (!opencl) {
                    pimpl_->compute_mode = OPENCL_MODE;
                    LOG_INFO_WITH_CONTEXT("OPENCL_MODE");
                    opencl = true;
                }
            }
        }

        if (opencl != opencl_old){
            opencl_old = opencl;
            if (opencl) {
                pimpl_->compute_mode = OPENCL_MODE;
                LOG_INFO_WITH_CONTEXT("OPENCL_MODE");
                if (opengl)
                    opengl = false;
            } else {
                if (!opengl){
                    pimpl_->compute_mode = OPENGL_MODE;
                    LOG_INFO_WITH_CONTEXT("OPENGL_MODE");
                    opengl = true;
                }
            }
        }




        // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            // counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // // 3. Show another simple window.
    // if (show_another_window)
    // {
    //     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //     ImGui::Text("Hello from another window!");
    //     if (ImGui::Button("Close Me"))
    //         show_another_window = false;
    //     ImGui::End();
    // }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(pimpl_->window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    // glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


}
