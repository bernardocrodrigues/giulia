#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <unistd.h>
#include "glutils.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

typedef enum {
    MOUSE_RELEASSED,
    MOUSE_JUST_CLICKED,
    MOUSE_CLICKED
} mouse_state_t ;

struct mouse_coodenate {
    double x;
    double y;
};

struct complex_number {
    double real;
    double imaginary;
};

mouse_state_t mouse = MOUSE_RELEASSED;
mouse_coodenate anchor = { 0.0, 0.0 };
mouse_coodenate delta = { 0.0, 0.0 };
complex_number down_left = {-2.2, -1.5};
double range_ = 3;
bool draw_request = true;

int width = 1000;
int height = 1000;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    switch (mouse) {
        case MOUSE_JUST_CLICKED:
            anchor = {xpos, ypos};
            mouse = MOUSE_CLICKED;
            break;
        case MOUSE_CLICKED:
            delta = { xpos - anchor.x, ypos - anchor.y};
            if (delta.x != 0 || delta.y != 0 ){
                down_left.real = down_left.real - (range_ * delta.x)/width;
                down_left.imaginary = down_left.imaginary + (range_ * delta.y)/height;
                anchor = {xpos, ypos};
                draw_request = true;
            }
            break;
        case MOUSE_RELEASSED:
            anchor = { 0.0, 0.0 };
            delta = { 0.0, 0.0 };
            break;
        default:
            break;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        // std::cout << "click " << std::endl;
        mouse = MOUSE_JUST_CLICKED;
    }else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        // std::cout << "rele " << std::endl;
        mouse = MOUSE_RELEASSED;
        draw_request = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

  if (yoffset > 0){
      range_ /=2;
      down_left = { down_left.real + range_/2, down_left.imaginary + range_/2};
      draw_request = true;

  }else if (yoffset < 0) {
      range_ *=2;
      down_left = { down_left.real - range_/4, down_left.imaginary - range_/4};
      draw_request = true;
  }
}

int main(void) {

    const char* glsl_version = "#version 130";

    GLFWwindow* window;
    // GLFWwindow* window2;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    // window2 = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSwapInterval(1);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK){
        std::cout << "GLEW ERROR!" << std::endl;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    auto gl_vendor = glGetString(GL_VENDOR);
    auto gl_renderer = glGetString(GL_RENDERER);
    auto gl_version = glGetString(GL_VERSION);
    auto gl_lang = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "Using OpenGL: \nVendor: " << gl_vendor << "\nRenderer: " << gl_renderer << "\nOpenGL Version: " << gl_version << "\nShader Lang: "<< gl_lang << std::endl;

    {

        float positions[] = {
            -1.0, -1.0,
            1.0, -1.0,
            1.0, 1.0,
            -1.0, 1.0
        };

        // float logo_size = 100;
        // // float logo_aspect = 18.0/43.0;
        // float logo_aspect = 1;
        // float logo_origin[] = {0.0F, 0.0F}; // x,y

        // float logo_positions[] = {
        //     logo_origin[0],                 logo_origin[1],                             0.0, 0.0,    // 0
        //     logo_origin[0] + logo_size,     logo_origin[1],                             1.0, 0.0,    // 1
        //     logo_origin[0] + logo_size,     logo_origin[1] + logo_aspect * logo_size,   1.0, 1.0,    // 2
        //     logo_origin[0],                 logo_origin[1] + logo_aspect * logo_size,    0.0, 1.0    // 3
        // };


        float logo_positions[] = {
            0,      0,      0.0,    0.0,    // 0
            430,    0,      1.0,    0.0,    // 1
            430,    180,    1.0,    1.0,    // 2
            0,      180,    0.0,    1.0     // 3
        };

        float logo_scale = 0.3;


        // float logo_positions[] = {
        //     0,       0,     0.0, 0.0,    // 0
        //     430,     0,     1.0, 0.0,    // 1
        //     430,     180,   1.0, 1.0,    // 2
        //     0,       180,   0.0, 1.0    // 3
        // };

        float cursor_origin[] = {0.0, 0.0}; // x,y
        float cursor_size = 0.02;
        float cursor_aspect = 0.86603;


        float cursor_position[] = {
            cursor_origin[0] + cursor_size/2,   cursor_aspect * cursor_size,   // 1
            cursor_origin[0] + cursor_size,     cursor_origin[1],   // 2
            cursor_origin[0],                   cursor_origin[1]    // 3
        };

        uint indices[] =
        { 0, 1, 2,
        2, 3, 0 };

        uint cursor_indices[] =
        { 0, 1, 2};

        VertexBufferLayout layout;
        layout.Push<float>(2);

        GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexBufferLayout texture_layout;
        texture_layout.Push<float>(2);
        texture_layout.Push<float>(2);

        VertexArray va;
        VertexBuffer vb(positions, 8 * sizeof(float));
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        VertexArray va_cursor;
        VertexBuffer vb_cursor(cursor_position, 6 * sizeof(float));
        va_cursor.AddBuffer(vb_cursor, layout);
        IndexBuffer ib_cursor(cursor_indices, 3);

        VertexArray va_texture;
        VertexBuffer vb_texture(logo_positions, 16 * sizeof(float));
        va_texture.AddBuffer(vb_texture, texture_layout);
        IndexBuffer ib_texture(indices, 6);


        glm::mat4 proj = glm::ortho(0.0, (double) width/logo_scale, 0.0, (double) height/logo_scale, 0.0, 2.0);
        glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(10,10,0));
        glm::mat4 mvp = proj * view;

        Shader double_precision_shader("src/gl/shaders/double_precision.shader");
        Shader double_precision_low_res("src/gl/shaders/double_precision_low_res.shader");
        Shader single_precision_shader("src/gl/shaders/single_precision.shader");
        Shader pointer_shader("src/gl/shaders/pointer.shader");
        Shader texture_shader("src/gl/shaders/texture.shader");

        double_precision_shader.Bind();
        double_precision_shader.SetUniform2ui("screen_resolution", width, height);
        double_precision_low_res.Bind();
        double_precision_low_res.SetUniform2ui("screen_resolution", width, height);
        single_precision_shader.Bind();
        single_precision_shader.SetUniform2ui("screen_resolution", width, height);
        single_precision_shader.SetUniform1ui("mode_", 0);

        texture_shader.Bind();
        Texture opengl_logo_texture("res/opengl_logo.png");
        opengl_logo_texture.Bind();
        texture_shader.SetUniform1i("u_texture", 0);
        texture_shader.SetUniformMat4f("u_MVP", mvp);

        Renderer renderer;

        double lastTime = glfwGetTime();
        int nbFrames = 0;


        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            GlCall(glfwPollEvents());



            double currentTime = glfwGetTime();
            nbFrames++;
            if ( currentTime - lastTime >= 1.0 ) {
                printf("%f ms/frame (%d fps) | range: %f\n", 1000.0/double(nbFrames), nbFrames, (float) range_);
                nbFrames = 0;
                lastTime += 1.0;
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            // /* Render here */
            if (draw_request){
            // ImGui_ImplOpenGL3_NewFrame();
            // ImGui_ImplGlfw_NewFrame();
            // ImGui::NewFrame();

                renderer.Clear();
                // std::cout << "aaa" << std::endl;

                if (range_ < 0.000023) {
                  if(mouse != MOUSE_RELEASSED){
                      double_precision_low_res.Bind();
                      double_precision_low_res.SetUniform2d("down_left", down_left.real, down_left.imaginary);
                      double_precision_low_res.SetUniform1d("range_", range_);
                      renderer.Draw(va, ib, double_precision_low_res);
                  }else{
                      double_precision_shader.Bind();
                      double_precision_shader.SetUniform2d("down_left", down_left.real, down_left.imaginary);
                      double_precision_shader.SetUniform1d("range_", range_);
                      renderer.Draw(va, ib, double_precision_shader);
                  }
                }else{
                    // double_precision_shader.Bind();
                    // double_precision_shader.SetUniform2d("down_left", down_left.real, down_left.imaginary);
                    // double_precision_shader.SetUniform1d("range_", range_);
                    // renderer.Draw(va, ib, double_precision_shader);
                    single_precision_shader.Bind();
                    single_precision_shader.SetUniform2f("down_left", (float) down_left.real, (float) down_left.imaginary);
                    single_precision_shader.SetUniform1f("range_", (float) range_);
                    renderer.Draw(va, ib, single_precision_shader);
                }

                GlCall(glEnable(GL_BLEND));
                renderer.Draw(va_cursor, ib_cursor, pointer_shader);
                renderer.Draw(va_texture, ib_texture, texture_shader);
                GlCall(glDisable(GL_BLEND));

                // {
                //     static float f = 0.0f;
                //     static int counter = 0;

                //     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                //     ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                //     ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                //     ImGui::Checkbox("Another Window", &show_another_window);

                //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                //     ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                //     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                //         counter++;
                //     ImGui::SameLine();
                //     ImGui::Text("counter = %d", counter);

                //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                //     ImGui::End();
                // }

                // // 3. Show another simple window.
                // if (show_another_window)
                // {
                //     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                //     ImGui::Text("Hello from another window!");
                //     if (ImGui::Button("Close Me"))
                //         show_another_window = false;
                //     ImGui::End();
                // }

                // // Rendering
                // ImGui::Render();
                // int display_w, display_h;
                // glfwGetFramebufferSize(window, &display_w, &display_h);
                // glViewport(0, 0, display_w, display_h);
                // // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
                // // glClear(GL_COLOR_BUFFER_BIT);
                // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


                // GlCall(glfwSwapBuffers(window));
                draw_request = false;
            } else {
                usleep(10000);
            }


                {
                    static float f = 0.0f;
                    static int counter = 0;

                    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                    ImGui::Checkbox("Another Window", &show_another_window);

                    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                        counter++;
                    ImGui::SameLine();
                    ImGui::Text("counter = %d", counter);

                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                    ImGui::End();
                }

                // 3. Show another simple window.
                if (show_another_window)
                {
                    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                    ImGui::Text("Hello from another window!");
                    if (ImGui::Button("Close Me"))
                        show_another_window = false;
                    ImGui::End();
                }

                // Rendering
                ImGui::Render();
                int display_w, display_h;
                glfwGetFramebufferSize(window, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                // glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
                // glClear(GL_COLOR_BUFFER_BIT);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            GlCall(glfwSwapBuffers(window));

            // shader.SetUniform2d("down_left", r, -1.5);
            // shader.SetUniform1d("range_", 3);

            /* Swap front and back buffers */

            /* Poll for and process events */


        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.

        // glfwSwapBuffers(window);
        }
    }
    glfwTerminate();
    return 0;
}