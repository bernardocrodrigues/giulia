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

#define iter 1024

layout(location = 0) out vec4 color;
uniform vec2 down_left;
uniform vec2 c;

uniform uvec2 screen_resolution;
uniform float range_x;
uniform float range_y;
uniform uint mode_;



float magnetude(in vec2 c_num){
    return sqrt((c_num.x * c_num.x) + (c_num.y * c_num.y));
}

vec2 product(in vec2 c_num_a, in vec2 c_num_b) {
    vec2 aux;
    aux.x = (c_num_a.x * c_num_b.x) - (c_num_a.y * c_num_b.y);
    aux.y = (c_num_a.x * c_num_b.y) + (c_num_a.y * c_num_b.x);
    return aux;
}

vec2 add_(in vec2 c_num_a, in vec2 c_num_b) {
    vec2 aux;
    aux.x = c_num_a.x + c_num_b.x;
    aux.y = c_num_a.y + c_num_b.y;
    return aux;
}

int mandebrot_set_degree(in vec2 z, in vec2 c, in int max_steps, in float threshold) {

    int index = 0;
    while (index < max_steps && (magnetude(z) < threshold)) {
        z = add_(product(z,z), c);
        index++;
    }
    return index;
}

void main()
{

    vec4 aux;
    vec2 z;
    int mandebrot_num;

    if (mode_ == 0) {
        z.x = down_left.x + (range_x*gl_FragCoord.x)/screen_resolution.x;
        z.y = down_left.y + (range_y*gl_FragCoord.y)/screen_resolution.y;
        mandebrot_num = mandebrot_set_degree(z, z, iter, 4.0);
    } else {
        z.x = down_left.x + ((range_x*gl_FragCoord.x)- screen_resolution.x * 2)/screen_resolution.x;
        z.y = down_left.y + (range_y*gl_FragCoord.y)/screen_resolution.y;
        mandebrot_num = mandebrot_set_degree(z, c, iter, 4.0);
    }

    float mandebrot = (float(mandebrot_num) / iter);
    aux.x = mandebrot;
    aux.y = mandebrot;
    aux.z = mandebrot;
    color = aux;
};
