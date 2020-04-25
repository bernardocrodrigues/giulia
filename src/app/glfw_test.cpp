#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include "glutils.hpp"



struct ShaderSource{
    std::string VertexSource;
    std::string FragmentSource;
};


static ShaderSource ParseShader(const std::string& filepath)
{
    enum class ShaderType{
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }else{
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

static int CompileShader(uint type, const std::string& source)
{
    uint id = glCreateShader(type);
    const char* src = source.c_str();
    GlCall(glShaderSource(id, 1, &src, nullptr));
    GlCall(glCompileShader(id));

    int result;
    GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result){
        int length;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char));
        GlCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " Shader!" << std::endl;
        std::cout << message << std::endl;
        GlCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

static uint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    uint program = glCreateProgram();
    uint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    uint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GlCall(glAttachShader(program, vs));
    GlCall(glAttachShader(program, fs));

    GlCall(glLinkProgram(program));
    GlCall(glValidateProgram(program));

    GlCall(glDeleteShader(vs));
    GlCall(glDeleteShader(fs));

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 1000, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

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

        uint vao;
        GlCall(glGenVertexArrays(1, &vao));
        GlCall(glBindVertexArray(vao));

        VertexBuffer vb(positions, 8 * sizeof(float));
        IndexBuffer ib(indices, 6);

        GlCall(glEnableVertexAttribArray(0));
        GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

        ShaderSource source = ParseShader("src/gl/shaders/basic.shader");

        uint shader = CreateShader(source.VertexSource, source.FragmentSource);
        GlCall(glUseProgram(shader));

        int location = glGetUniformLocation(shader, "u_color");
        ASSERT(location != -1);
        GlCall(glUniform4f(location, 0.2, 0.3, 0.8, 1.0));

        float r = 0.0;
        float inc = 0.05;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GlCall(glClear(GL_COLOR_BUFFER_BIT));

            if(r > 1.0){
                inc = -0.05;
            }else if( r < 0.0){
                inc = 0.05;
            }

            r += inc;

            GlCall(glUniform4f(location, r, 0.3, 0.8, 1.0));
            GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            /* Swap front and back buffers */
            GlCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GlCall(glfwPollEvents());
        }

        GlCall(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}