#shader vertex
#version 400 core

layout(location = 0) in vec4 position;
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
};

#shader fragment
#version 400 core

layout(location = 0) out vec4 color;


void main()
{
    color = vec4(1.0, 0.0, 0.0, 0.9);
};
