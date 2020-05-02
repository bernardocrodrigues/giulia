#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include "glutils.hpp"

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


int width = 2000;
int height = 2000;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    switch (mouse)
    {
    case MOUSE_JUST_CLICKED:
        anchor = {xpos, ypos};
        mouse = MOUSE_CLICKED;
        break;
    case MOUSE_CLICKED:
        delta = { xpos - anchor.x, ypos - anchor.y};
        down_left.real = down_left.real - (range_ * delta.x)/width;
        down_left.imaginary = down_left.imaginary + (range_ * delta.y)/height;
        anchor = {xpos, ypos};
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
        std::cout << "click " << std::endl;
        mouse = MOUSE_JUST_CLICKED;
    }else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        std::cout << "rele " << std::endl;
        mouse = MOUSE_RELEASSED;

    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

  if (yoffset > 0){
      range_ /=2;
      down_left = { down_left.real + range_/2, down_left.imaginary + range_/2};
  }else if (yoffset < 0) {
      range_ *=2;
      down_left = { down_left.real - range_/4, down_left.imaginary - range_/4};
  }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
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

        float position_cursor[] = {
            0.0, 0.5,
            0.5, -0.5,
            -0.5, -0.5,
        };

        uint indices[] =
        { 0, 1, 2,
        2, 3, 0 };

        uint indices_cursor[] =
        { 0, 1, 2};

        VertexBufferLayout layout;
        layout.Push<float>(2);

        VertexArray va;
        VertexBuffer vb(positions, 8 * sizeof(float));
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        VertexArray va_cursor;
        VertexBuffer vb_cursor(position_cursor, 6 * sizeof(float));
        va_cursor.AddBuffer(vb_cursor, layout);
        IndexBuffer ib_cursor(indices_cursor, 3);

        Shader double_precision_shader("src/gl/shaders/double_precision.shader");
        Shader single_precision_shader("src/gl/shaders/single_precision.shader");
        Shader pointer_shader("src/gl/shaders/pointer.shader");

        double_precision_shader.Bind();
        double_precision_shader.SetUniform2ui("screen_resolution", width, height);
        double_precision_shader.SetUniform1ui("mode_", 0);
        single_precision_shader.Bind();
        single_precision_shader.SetUniform2ui("screen_resolution", width, height);
        single_precision_shader.SetUniform1ui("mode_", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        double lastTime = glfwGetTime();
        int nbFrames = 0;


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            double currentTime = glfwGetTime();
            nbFrames++;
            if ( currentTime - lastTime >= 1.0 ) {
                printf("%f ms/frame (%d fps) | range: %f\n", 1000.0/double(nbFrames), nbFrames, (float) range_);
                nbFrames = 0;
                lastTime += 1.0;
            }

            /* Render here */
            renderer.Clear();

            if (range_ < 0.000023) {
                double_precision_shader.Bind();
                double_precision_shader.SetUniform2d("down_left", down_left.real, down_left.imaginary);
                double_precision_shader.SetUniform1d("range_", range_);
                renderer.Draw(va, ib, double_precision_shader);
            }else{
                single_precision_shader.Bind();
                single_precision_shader.SetUniform2f("down_left", (float) down_left.real, (float) down_left.imaginary);
                single_precision_shader.SetUniform1f("range_", (float) range_);
                renderer.Draw(va, ib, single_precision_shader);
            }

            // renderer.Draw(va_cursor, ib_cursor, pointer_shader);

            // shader.SetUniform2d("down_left", r, -1.5);
            // shader.SetUniform1d("range_", 3);

            /* Swap front and back buffers */
            GlCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GlCall(glfwPollEvents());
        }

    }
    glfwTerminate();
    return 0;
}