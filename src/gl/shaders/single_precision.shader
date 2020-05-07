#shader vertex
#version 400 core

layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position;
};

#shader fragment
#version 400 core

#define iter 1024

layout(location = 0) out vec4 color;
uniform vec2 down_left;
uniform uvec2 screen_resolution;
uniform float range_;
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
    if (mode_ == 0) {
        vec2 z, c;
        z.x = down_left.x + (range_*gl_FragCoord.x)/screen_resolution.x;
        z.y = down_left.y + (range_*gl_FragCoord.y)/screen_resolution.y;

        int mandebrot_num = mandebrot_set_degree(z, z, iter, 4.0);
        float mandebrot = (float(mandebrot_num) / iter);
        aux.x = mandebrot;
        aux.y = mandebrot;
        aux.z = mandebrot;
    } else if ( mode_ == 1) {
        vec2 z, c;
        z.x = -1.5 + (range_*gl_FragCoord.x)/screen_resolution.x;
        z.y = -1.5 + (range_*gl_FragCoord.y)/screen_resolution.y;

        int mandebrot_num = mandebrot_set_degree(z, down_left, iter, 4.0);
        float mandebrot = (float(mandebrot_num) / iter);
        aux.x = mandebrot;
        aux.y = mandebrot;
        aux.z = mandebrot;
    }

    color = aux;
};
