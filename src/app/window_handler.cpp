
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
        precision_mode_t precision_mode;
        fullscreen_mode_t fullscreen_mode;

        mouse_state_t mouse;
        mouse_coodenate mouse_position;
        mouse_coodenate anchor;
        window_region_t selected_window;

        bool pan;
        bool render_request;

        complex_number position_on_left_window;
        complex_number position_on_right_window;
        complex_number cursor_position;
        double range_x_on_left_window;
        double range_x_on_right_window;
        float aspect_ratio;

        int iter;
        int color_preset;
        int exponent;

        Pimpl();
        ~Pimpl();
};

Pimpl::Pimpl() :
    imgui_focus(false),
    compute_mode(OPENGL_MODE),
    precision_mode(SINGLE_MODE),
    mouse(MOUSE_RELEASSED),
    pan(false),
    render_request(true),
    range_x_on_left_window(3),
    range_x_on_right_window(3),
    aspect_ratio(1),
    iter(32),
    color_preset(1),
    exponent(2)
    {}

static void glfw_error_callback(int error, const char* desc) {
    (void) error;
    (void) desc;
    fputs(desc,stderr);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action,
                                  int mods) {
  (void)mods;
  double xpos, ypos;
  Pimpl* pimpl_ = reinterpret_cast<Pimpl*>(glfwGetWindowUserPointer(window));

  if (!pimpl_->imgui_focus) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      glfwGetCursorPos(window, &xpos, &ypos);
      if (pimpl_->pan) {
        if (action == GLFW_PRESS) {
          pimpl_->mouse_position = {xpos, HEIGHT - ypos};
          if (xpos > WIDTH / 2) {
            pimpl_->selected_window = RIGHT;
          } else {
            pimpl_->selected_window = LEFT;
          }
          pimpl_->mouse = MOUSE_JUST_CLICKED;
        } else if (action == GLFW_RELEASE) {
          pimpl_->render_request = true;
          pimpl_->mouse = MOUSE_RELEASSED;
        }
      } else {
          if (xpos < WIDTH / 2) {
              if (action == GLFW_PRESS) {
                pimpl_->cursor_position.real = pimpl_->position_on_left_window.real + ((pimpl_->range_x_on_left_window * xpos)/WIDTH*2);
                pimpl_->cursor_position.imaginary = pimpl_->position_on_left_window.imaginary + (pimpl_->range_x_on_left_window * pimpl_->aspect_ratio *
                                                                        (HEIGHT - ypos))/HEIGHT;
                pimpl_->mouse = MOUSE_JUST_CLICKED;
              } else if (action == GLFW_RELEASE) {
                pimpl_->mouse = MOUSE_RELEASSED;
                pimpl_->render_request = true;
              }
            // LOG_INFO( "X: " << pimpl_->cursor_position.real << " Y: " << pimpl_->cursor_position.imaginary);
          }
      }
    }
  }
}

static void cursor_position_callback(GLFWwindow* window, double xpos,
                                     double ypos) {
  Pimpl* pimpl_ = reinterpret_cast<Pimpl*>(glfwGetWindowUserPointer(window));

  if (!pimpl_->imgui_focus) {
    switch (pimpl_->mouse) {
      case MOUSE_JUST_CLICKED:
        pimpl_->anchor = {xpos, ypos};
        pimpl_->mouse = MOUSE_CLICKED;
        break;
      case MOUSE_CLICKED: {
        if (pimpl_->pan) {
          mouse_coodenate delta = {xpos - pimpl_->anchor.x,
                                   ypos - pimpl_->anchor.y};
          if (delta.x != 0 || delta.y != 0) {
            if (pimpl_->selected_window == LEFT) {
              pimpl_->position_on_left_window.real =
                  pimpl_->position_on_left_window.real -
                  (pimpl_->range_x_on_left_window * delta.x) / WIDTH * 2;
              pimpl_->position_on_left_window.imaginary =
                  pimpl_->position_on_left_window.imaginary +
                  (pimpl_->range_x_on_left_window * pimpl_->aspect_ratio * delta.y) / HEIGHT;
            } else {
              pimpl_->position_on_right_window.real =
                  pimpl_->position_on_right_window.real -
                  (pimpl_->range_x_on_right_window * delta.x) / WIDTH * 2;
              pimpl_->position_on_right_window.imaginary =
                  pimpl_->position_on_right_window.imaginary +
                  (pimpl_->range_x_on_right_window * pimpl_->aspect_ratio * delta.y) / HEIGHT;
            }
            pimpl_->anchor = {xpos, ypos};
            pimpl_->render_request = true;
            }
          } else {
            if (xpos < WIDTH / 2) {
              pimpl_->cursor_position.real =
                  pimpl_->position_on_left_window.real +
                  ((pimpl_->range_x_on_left_window * xpos) / WIDTH * 2);
              pimpl_->cursor_position.imaginary =
                  pimpl_->position_on_left_window.imaginary +
                  (pimpl_->range_x_on_left_window * pimpl_->aspect_ratio * (HEIGHT - ypos)) /
                      HEIGHT;
            }
          }
      } break;
      case MOUSE_RELEASSED:
        pimpl_->anchor = {0.0, 0.0};
        break;
      default:
        break;
    }
  } else {
    if (pimpl_->mouse == MOUSE_CLICKED || pimpl_->mouse == MOUSE_JUST_CLICKED)
      pimpl_->render_request = true;
  }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) scancode;
    (void) mods;
    Pimpl *pimpl_ = reinterpret_cast<Pimpl*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_SPACE ) {
        if (action == GLFW_PRESS) {
            pimpl_->pan = true;
            // LOG_INFO("aaa");
        } else if(action == GLFW_RELEASE){
            // LOG_INFO("bbb");
            pimpl_->pan = false;
            pimpl_->mouse = MOUSE_RELEASSED;
        }
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

    double xpos, ypos;
    Pimpl* pimpl_ = reinterpret_cast<Pimpl*>(glfwGetWindowUserPointer(window));
    glfwGetCursorPos(window, &xpos, &ypos);

    float ratio;
    float offset;

    if (yoffset > 0) {
        ratio = 0.5;
    } else if (yoffset < 0) {
        ratio = 2;
    }

    if (xpos <= WIDTH / 2) {
        if (yoffset > 0) {
        pimpl_->range_x_on_left_window /= 2;
        pimpl_->position_on_left_window = {
            pimpl_->position_on_left_window.real +
                pimpl_->range_x_on_left_window / 2,
            pimpl_->position_on_left_window.imaginary +
                pimpl_->range_x_on_left_window * pimpl_->aspect_ratio / 2};
        } else if (yoffset < 0) {
        pimpl_->range_x_on_left_window *= 2;
        pimpl_->position_on_left_window = {
            pimpl_->position_on_left_window.real -
                pimpl_->range_x_on_left_window / 4,
            pimpl_->position_on_left_window.imaginary -
                pimpl_->range_x_on_left_window * pimpl_->aspect_ratio / 4};
        }
        pimpl_->render_request = true;
    } else {
        if (yoffset > 0) {
        pimpl_->range_x_on_right_window /= 2;
        pimpl_->position_on_right_window = {
            pimpl_->position_on_right_window.real +
                pimpl_->range_x_on_right_window / 2,
            pimpl_->position_on_right_window.imaginary +
                pimpl_->range_x_on_right_window * pimpl_->aspect_ratio / 2};
        } else if (yoffset < 0) {
        pimpl_->range_x_on_right_window *= 2;
        pimpl_->position_on_right_window = {
            pimpl_->position_on_right_window.real -
                pimpl_->range_x_on_right_window / 4,
            pimpl_->position_on_right_window.imaginary -
                pimpl_->range_x_on_right_window * pimpl_->aspect_ratio / 4};
        }
        pimpl_->render_request = true;
    }
    LOG_INFO("LEFT " << pimpl_->range_x_on_left_window << " " << pimpl_->position_on_left_window.real << " " << pimpl_->position_on_left_window.imaginary);
    LOG_INFO("RIGHT " << pimpl_->range_x_on_right_window << " " << pimpl_->position_on_right_window.real << " " << pimpl_->position_on_right_window.imaginary);
}


Handler::Handler(complex_number start_position_on_left, complex_number start_position_on_right, complex_number cursor_start_position):
    pimpl_(new Pimpl()) {

    pimpl_->position_on_left_window = start_position_on_left;
    pimpl_->position_on_right_window = start_position_on_right;
    pimpl_->cursor_position = cursor_start_position;

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
    glfwSetCursorPosCallback(pimpl_->window, cursor_position_callback);
    glfwSetKeyCallback(pimpl_->window, key_callback);
    glfwSetScrollCallback(pimpl_->window, scroll_callback);


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
precision_mode_t Handler::get_selected_precision_mode() { return pimpl_->precision_mode; }


complex_number Handler::get_left_position() {
    return pimpl_->position_on_left_window;
}

complex_number Handler::get_right_position() {
    return pimpl_->position_on_right_window;
}

complex_number Handler::get_cursor_position() {
    return pimpl_->cursor_position;
}

double Handler::get_x_range_on_right() {
    return pimpl_->range_x_on_right_window;
}

double Handler::get_x_range_on_left() {
    return pimpl_->range_x_on_left_window;
}


void Handler::render_imgui() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        static bool opengl = true;
        static bool opengl_old = true;
        static bool opencl = false;
        static bool opencl_old = false;

        static bool single_p = true;
        static bool single_p_old = true;
        static bool double_p = false;
        static bool double_p_old = false;

        static bool fullsceen_mandelbrot = true;
        static bool fullsceen_mandelbrot_old = true;
        static bool fullsceen_julia = false;
        static bool fullsceen_julia_old = false;

        ImGui::Begin("Settings");                          // Create a window called "Hello, world!" and append into it.

        pimpl_->imgui_focus = ImGui::IsWindowHovered();

        ImGui::Text("Compute Mode: ");
        ImGui::SameLine();
        ImGui::Checkbox("OpenGL", &opengl);
        ImGui::SameLine();
        ImGui::Checkbox("OpenCL", &opencl);

        ImGui::Text("Precision: ");
        ImGui::SameLine();
        ImGui::Checkbox("Single", &single_p);
        ImGui::SameLine();
        ImGui::Checkbox("Double", &double_p);

        ImGui::Text("Full Screen: ");
        ImGui::SameLine();
        ImGui::Checkbox("Mandelbrot", &fullsceen_mandelbrot);
        ImGui::SameLine();
        ImGui::Checkbox("Julia", &fullsceen_julia);

        ImGui::SliderInt("Iterations", &pimpl_->iter, 0, 2000);
        ImGui::SliderInt("Color Preset", &pimpl_->color_preset, 1, 5);
        ImGui::SliderInt("Z's exponent", &pimpl_->exponent, 2, 30);

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


        if (single_p != single_p_old) {
            single_p_old = single_p;
            if (single_p) {
                pimpl_->precision_mode = SINGLE_MODE;
                LOG_INFO_WITH_CONTEXT("SINGLE_PRECISION_MODE");
                if (double_p)
                    double_p = false;
            } else {
                if (!double_p) {
                    pimpl_->precision_mode = DOUBLE_MODE;
                    LOG_INFO_WITH_CONTEXT("DOUBLE_PRECISION_MODE");
                    double_p = true;
                }
            }
        }

        if (double_p != double_p_old){
            double_p_old = double_p;
            if (double_p) {
                pimpl_->precision_mode = DOUBLE_MODE;
                LOG_INFO_WITH_CONTEXT("DOUBLE_PRECISION_MODE");
                if (single_p)
                    single_p = false;
            } else {
                if (!single_p){
                    pimpl_->precision_mode = SINGLE_MODE;
                    LOG_INFO_WITH_CONTEXT("SINGLE_PRECISION_MODE");
                    single_p = true;
                }
            }
        }

        if (fullsceen_mandelbrot != fullsceen_mandelbrot_old) {
            fullsceen_mandelbrot_old = fullsceen_mandelbrot;
            if (fullsceen_mandelbrot) {
                pimpl_->fullscreen_mode = MANDELBROT_F;
                LOG_INFO_WITH_CONTEXT("FULL_SCREEN_MANDELBROT");
                if (fullsceen_julia)
                    fullsceen_julia = false;
            }
        }

        if (fullsceen_julia != fullsceen_julia_old){
            fullsceen_julia_old = fullsceen_julia;
            if (fullsceen_julia) {
                pimpl_->fullscreen_mode = JULIA_F;
                LOG_INFO_WITH_CONTEXT("FULL_SCREEN_JULIA");
                if (fullsceen_mandelbrot)
                    fullsceen_mandelbrot = false;
            }
        }

        if(!fullsceen_julia && !fullsceen_mandelbrot) {
            pimpl_->fullscreen_mode = NONE;
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

int Handler::get_iter_number() {
    return pimpl_->iter;
}

int Handler::get_color_preset_number() {
    return pimpl_->color_preset;
}

int Handler::get_exponent() {
    return pimpl_->exponent;
}

fullscreen_mode_t Handler::get_fullscreen_mode() {
    return pimpl_->fullscreen_mode;
}

bool Handler::render_requested() {
    if (pimpl_->render_request) {
        pimpl_->render_request = false;
        return true;
    } else {
        return false;
    }
}

}