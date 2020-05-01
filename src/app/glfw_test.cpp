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


int width = 1000;
int height = 1000;

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

        uint indices[] =
        { 0, 1, 2,
        2, 3, 0 };

        VertexArray va;
        VertexBuffer vb(positions, 8 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("src/gl/shaders/basic.shader");
        shader.Bind();
        // shader.SetUniform2d("down_left", -2.2, -1.5);
        // shader.SetUniform1d("range_", 3);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        // float r = -3.0;
        float inc = 0.05;

        double lastTime = glfwGetTime();
        int nbFrames = 0;


        
        // complex_number down_left = {-2.2, -1.5};

        // double xpos, ypos;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            double currentTime = glfwGetTime();
            nbFrames++;
            if ( currentTime - lastTime >= 1.0 ) {
                printf("%f ms/frame (%d fps)\n", 1000.0/double(nbFrames), nbFrames);
                nbFrames = 0;
                lastTime += 1.0;
            }

            // if(r > 3.0){
            //     inc = -0.05;
            // }else if( r < 0.5){
            //     inc = 0.05;
            // }
            // r += inc;



            shader.Bind();
            shader.SetUniform2d("down_left", down_left.real, down_left.imaginary);
            shader.SetUniform1d("range_", range_);
            /* Render here */
            renderer.Clear();

            // shader.SetUniform2d("down_left", r, -1.5);
            // shader.SetUniform1d("range_", 3);


            renderer.Draw(va, ib, shader);

            /* Swap front and back buffers */
            GlCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GlCall(glfwPollEvents());
        }

    }
    glfwTerminate();
    return 0;
}