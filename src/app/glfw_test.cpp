#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <unistd.h>
#include "data_def.hpp"

#include "clutils.hpp"
#include "glutils.hpp"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

mouse_state_t mouse = MOUSE_RELEASSED;
mouse_coodenate anchor = { 0.0, 0.0 };
mouse_coodenate delta = { 0.0, 0.0 };

complex_number down_left = {-2.2, -1.5};

mouse_coodenate cursor_screen = { 0.0, 0.0 };
complex_number cursor_selection = {0.0, 0.0};

int draw_request = 2;
bool hover = false;

// int width = 2000;
// int height = 1000;

// double aspect_ratio = ((double)height / (double)width);
double aspect_ratio = 1;

double range_x = 3;
double range_y = range_x * aspect_ratio;

static complex_number get_complex_num_from_coordanate(mouse_coodenate& point,
                                                      complex_number& down_left,
                                                      double range_x,
                                                      double width,
                                                      double aspect_ratio) {

    double range_y = range_x * aspect_ratio;
    double height = width * aspect_ratio;

    return complex_number { down_left.real + (point.x * range_x)/width,
                            down_left.imaginary + (point.y * range_y)/height };

}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if(!hover){
        switch (mouse) {
            case MOUSE_JUST_CLICKED:
                // std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa " << std::endl;

                anchor = {xpos, ypos};
                mouse = MOUSE_CLICKED;
                break;
            case MOUSE_CLICKED:
                delta = { xpos - anchor.x, ypos - anchor.y};
                if (delta.x != 0 || delta.y != 0 ){
                    // down_left.real = down_left.real - (range_x * delta.x)/width;
                    // down_left.imaginary = down_left.imaginary + (range_y * delta.y)/height;

                    // cursor_selection.real = cursor_selection.real - (range_x * delta.x)/width;
                    // cursor_selection.imaginary = cursor_selection.imaginary + (range_y * delta.y)/height;

                    cursor_screen = {cursor_screen.x + delta.x, cursor_screen.y - delta.y};

                    cursor_selection = get_complex_num_from_coordanate(cursor_screen,
                                                                       down_left,
                                                                       range_x,
                                                                       WIDTH/2,
                                                                       aspect_ratio);

                    anchor = {xpos, ypos};
                    draw_request = 1;
                }
                break;
            case MOUSE_RELEASSED:
                anchor = { 0.0, 0.0 };
                delta = { 0.0, 0.0 };
                break;
            default:
                break;
        }
    }else{
        if(mouse == MOUSE_CLICKED || mouse == MOUSE_JUST_CLICKED)
            draw_request = 1;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        std::cout << "click " << std::endl;

        if (!hover){
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            cursor_screen = {xpos, HEIGHT - ypos};
        }
        mouse = MOUSE_JUST_CLICKED;
    }else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        // std::cout << "rele " << std::endl;
        mouse = MOUSE_RELEASSED;
        draw_request = 2;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

  if (yoffset > 0){
      range_x /= 2;
      range_y /= 2;
      down_left = { down_left.real + range_x/2, down_left.imaginary + range_y/2};
      draw_request = 2;

  }else if (yoffset < 0) {
      range_x *=2;
      range_y *=2;
      down_left = { down_left.real - range_x/4, down_left.imaginary - range_y/4};
      draw_request = 2;
  }
}

void render_imgui(GLFWwindow* window, bool& hover) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        hover = ImGui::IsWindowFocused();
        // ImGuiIO *io = &ImGui::GetIO();

        // io.WantCaptureMous = false;
        // hover = io->MouseDown[0];


        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

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
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    // glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


int main(void) {

    const char* glsl_version = "#version 130";

    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSwapInterval(1);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK){
        std::cout << "GLEW ERROR!" << std::endl;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    cl_int err = 0;
    auto program = create_program("src/cl/kernels/fractal.cl",
                                  (cl_context_properties)glfwGetGLXContext(window),
                                  (cl_context_properties)glfwGetX11Display());

    auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto device = program.getInfo<CL_PROGRAM_DEVICES>().front();
    cl::Kernel kernel(program, "Fractal", &err);

    std::cout << err << std::endl;

    auto gl_vendor = glGetString(GL_VENDOR);
    auto gl_renderer = glGetString(GL_RENDERER);
    auto gl_version = glGetString(GL_VERSION);
    auto gl_lang = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "Using OpenGL: \nVendor: " << gl_vendor << "\nRenderer: " << gl_renderer << "\nOpenGL Version: " << gl_version << "\nShader Lang: "<< gl_lang << std::endl;

    {

        GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexBufferLayout layout;
        layout.Push<float>(2);

        VertexBufferLayout texture_layout;
        texture_layout.Push<float>(2);
        texture_layout.Push<float>(2);

        VertexArray left_half_va;
        VertexBuffer left_half_vb(left_half_positions, 8 * sizeof(float));
        left_half_va.AddBuffer(left_half_vb, layout);
        IndexBuffer left_half_ib(indices, 6);

        VertexArray right_half_va;
        VertexBuffer right_half_vb(right_half_positions, 8 * sizeof(float));
        right_half_va.AddBuffer(right_half_vb, layout);
        IndexBuffer right_half_ib(indices, 6);

        VertexArray full_screen_va;
        VertexBuffer full_screen_vb(full_screen_positions, 8 * sizeof(float));
        full_screen_va.AddBuffer(full_screen_vb, layout);
        IndexBuffer full_screen_ib(indices, 6);

        VertexArray va_cursor;
        VertexBuffer vb_cursor(cursor_position, 6 * sizeof(float));
        va_cursor.AddBuffer(vb_cursor, layout);
        IndexBuffer ib_cursor(cursor_indices, 3);

        VertexArray va_texture;
        VertexBuffer vb_texture(logo_positions, 16 * sizeof(float));
        va_texture.AddBuffer(vb_texture, texture_layout);
        IndexBuffer ib_texture(indices, 6);

        glm::mat4 proj_screen = glm::ortho(0.0, (double) WIDTH, 0.0, (double) HEIGHT, 0.0, 2.0);
        glm::mat4 proj_cursor = glm::ortho(0.0, (double) WIDTH/cursor_scale, 0.0, (double) HEIGHT/cursor_scale, 0.0, 2.0);

        glm::mat4 proj = glm::ortho(0.0, (double) WIDTH/logo_scale, 0.0, (double) HEIGHT/logo_scale, 0.0, 2.0);
        glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(10,10,0));
        glm::mat4 mvp = proj * view;

        Shader double_precision_shader("src/gl/shaders/double_precision.shader");
        Shader double_precision_low_res("src/gl/shaders/double_precision_low_res.shader");
        Shader single_precision_shader("src/gl/shaders/single_precision.shader");
        Shader pointer_shader("src/gl/shaders/pointer.shader");
        Shader texture_shader("src/gl/shaders/texture.shader");

        double_precision_shader.Bind();
        double_precision_shader.SetUniform2ui("screen_resolution", WIDTH, HEIGHT);
        double_precision_shader.SetUniformMat4f("u_MVP", proj_screen);
        double_precision_low_res.Bind();
        double_precision_low_res.SetUniform2ui("screen_resolution", WIDTH, HEIGHT);
        double_precision_low_res.SetUniformMat4f("u_MVP", proj_screen);
        single_precision_shader.Bind();
        single_precision_shader.SetUniform2ui("screen_resolution", WIDTH/2, HEIGHT);
        single_precision_shader.SetUniformMat4f("u_MVP", proj_screen);
        single_precision_shader.SetUniform1ui("mode_", 0);

        texture_shader.Bind();
        // Texture opengl_logo_texture("res/opengl_logo.png");
        // opengl_logo_texture.Bind();
        texture_shader.SetUniform1i("u_texture", 0);
        texture_shader.SetUniformMat4f("u_MVP", mvp);

        cl::CommandQueue queue(context, device);
        cl_int errCode;
        Texture cl_texture(100, 100, 10);
        // cl_mem mem = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, cl_texture.GetRenderId() ,NULL);
        cl::ImageGL tex(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, cl_texture.GetRenderId() ,&errCode);
        // cl_texture.Unbind();

        std::cout << err << std::endl;

        try {

            glFinish();
            cl::Event ev;
            std::vector<cl::Memory> objs;
            objs.clear();
            objs.push_back(tex);
            cl_int res = queue.enqueueAcquireGLObjects(&objs, NULL, &ev);
            ev.wait();
            if (res!=CL_SUCCESS) {
                std::cout<<"Failed acquiring GL object: "<<res<<std::endl;
                exit(248);
            }

            cl::NDRange local(10, 10);
            cl::NDRange global(100, 100);

            kernel.setArg(0, tex);
            queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

            queue.enqueueReleaseGLObjects(&objs);
            ev.wait();
            if (res!=CL_SUCCESS) {
                std::cout<<"Failed releasing GL object: "<<res<<std::endl;
                exit(247);
            }
            queue.finish();
            } catch(cl::Error err) {
        std::cout << err.what() << "(" << err.err() << ")" << std::endl;
    }
        // queue.finish();

        // glFinish();
        // queue.enqueueAcquireGLObjects(&objs, NULL, &ev);
        // kernel.setArg(0, tex);
        // queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
        // queue.enqueueReleaseGLObjects(&objs, NULL, &ev);
        // glFinish();
        // queue.finish();

        pointer_shader.Bind();
        pointer_shader.SetUniformMat4f("u_MVP", proj_cursor);


        Renderer renderer;

        double lastTime = glfwGetTime();
        int nbFrames = 0;


        // bool show_demo_window = true;
        // bool show_another_window = false;
        // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // bool just_rendered = false;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GlCall(glfwPollEvents());


            // // /* Render here */
            // if (draw_request){
            //     draw_request--;
            //     renderer.Clear();
            //     // std::cout << "aaa" << std::endl;

            //     if (range_x < 0.000023) {
            //       if(mouse != MOUSE_RELEASSED){
            //           double_precision_low_res.Bind();
            //           double_precision_low_res.SetUniform2d("down_left", down_left.real, down_left.imaginary);
            //           double_precision_low_res.SetUniform1d("range_x", range_x);
            //           double_precision_low_res.SetUniform1d("range_y", range_y);
            //           renderer.Draw(left_half_va, left_half_ib, double_precision_low_res);
            //       }else{
            //           double_precision_shader.Bind();
            //           double_precision_shader.SetUniform2d("down_left", down_left.real, down_left.imaginary);
            //           double_precision_shader.SetUniform1d("range_x", range_x);
            //           double_precision_shader.SetUniform1d("range_y", range_y);
            //           renderer.Draw(left_half_va, left_half_ib, double_precision_shader);
            //       }
            //     }else{
            //         // double_precision_shader.Bind();
            //         // double_precision_shader.SetUniform2d("down_left", down_left.real, down_left.imaginary);
            //         // double_precision_shader.SetUniform1d("range_", range_);
            //         // renderer.Draw(va, ib, double_precision_shader);
            //         single_precision_shader.Bind();
            //         single_precision_shader.SetUniform1ui("mode_", 0);
            //         single_precision_shader.SetUniform2f("down_left", (float) down_left.real, (float) down_left.imaginary);
            //         single_precision_shader.SetUniform1f("range_x", (float) range_x);
            //         single_precision_shader.SetUniform1f("range_y", (float) range_y);
            //         renderer.Draw(left_half_va, left_half_ib, single_precision_shader);

            //         single_precision_shader.SetUniform1ui("mode_", 1);
            //         single_precision_shader.SetUniform2f("c", (float) cursor_selection.real, (float) cursor_selection.imaginary);
            //         renderer.Draw(right_half_va, right_half_ib, single_precision_shader);

            //     }

            //     GlCall(glEnable(GL_BLEND));

            //     glm::mat4 result = proj_cursor * glm::translate(glm::mat4(1.0), glm::vec3(cursor_screen.x/cursor_scale, (cursor_screen.y/cursor_scale), 0));
            //     pointer_shader.Bind();
            //     pointer_shader.SetUniformMat4f("u_MVP", result);
            //     renderer.Draw(va_cursor, ib_cursor, pointer_shader);




            //     renderer.Draw(va_texture, ib_texture, texture_shader);
            //     GlCall(glDisable(GL_BLEND));

            // } else {
            //     usleep(10000);
            // }

            // std::cout << cursor_screen.x << ":" << cursor_screen.y << "   " << cursor_selection.real << ":" << cursor_selection.imaginary  << std::endl;


            // GlCall(glEnable(GL_BLEND));
            // opengl_logo_texture.Bind();
            cl_texture.Bind();
            // texture_shader.Bind();
            // texture_shader.SetUniform1i("u_texture", 1);
            renderer.Draw(va_texture, ib_texture, texture_shader);
            // GlCall(glDisable(GL_BLEND));

            render_imgui(window, hover);


            GlCall(glfwSwapBuffers(window));

            // shader.SetUniform2d("down_left", r, -1.5);
            // shader.SetUniform1d("range_", 3);

            /* Swap front and back buffers */

            /* Poll for and process events */


        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.

        // glfwSwapBuffers(window);
        }
    }
    // glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}