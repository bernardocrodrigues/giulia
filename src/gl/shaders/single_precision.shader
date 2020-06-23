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

// #define iter 32

layout(location = 0) out vec4 color;
uniform dvec2 down_left;
uniform dvec2 c;

uniform uvec2 render_resolution;
uniform uvec2 render_offset;
uniform float range_x;
uniform float range_y;
uniform uint mode_;
uniform uint iter;



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
    vec2 c_single;
    c_single.x = float(c.x);
    c_single.y = float(c.y);
    int mandebrot_num;

    if (mode_ == 0) {
        z.x = float(down_left.x) + (range_x*(gl_FragCoord.x - render_offset.x))/render_resolution.x;
        z.y = float(down_left.y) + (range_y*gl_FragCoord.y)/render_resolution.y;
        mandebrot_num = mandebrot_set_degree(z, z, int(iter), 4.0);
    } else {
        z.x = float(down_left.x) + ((range_x*gl_FragCoord.x) - render_resolution.x * 2)/render_resolution.x;
        z.y = float(down_left.y) + (range_y*gl_FragCoord.y)/render_resolution.y;
        mandebrot_num = mandebrot_set_degree(z, c_single, int(iter), 4.0);
    }

    float mandebrot = (float(mandebrot_num) /  int(iter));
    aux.x = mandebrot;
    aux.y = mandebrot;
    aux.z = mandebrot;
    color = aux;
};
