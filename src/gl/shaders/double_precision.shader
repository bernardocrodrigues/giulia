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
uniform dvec2 down_left;
uniform dvec2 c;

uniform uvec2 render_resolution;
uniform uvec2 render_offset;
uniform float range_x;
uniform float range_y;
uniform uint mode_;
uniform uint iter;

double magnetude(in dvec2 c_num){
    return sqrt((c_num.x * c_num.x) + (c_num.y * c_num.y));
}

dvec2 product(in dvec2 c_num_a, in dvec2 c_num_b) {
    dvec2 aux;
    aux.x = (c_num_a.x * c_num_b.x) - (c_num_a.y * c_num_b.y);
    aux.y = (c_num_a.x * c_num_b.y) + (c_num_a.y * c_num_b.x);
    return aux;
}

dvec2 add_(in dvec2 c_num_a, in dvec2 c_num_b) {
    dvec2 aux;
    aux.x = c_num_a.x + c_num_b.x;
    aux.y = c_num_a.y + c_num_b.y;
    return aux;
}

int mandebrot_set_degree(in dvec2 z, in dvec2 c, in int max_steps, in float threshold) {

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
    dvec2 z;
    int mandebrot_num;

    if (mode_ == 0) {
        z.x = down_left.x + (range_x*(gl_FragCoord.x - render_offset.x))/render_resolution.x;
        z.y = down_left.y + (range_y*gl_FragCoord.y)/render_resolution.y;
        mandebrot_num = mandebrot_set_degree(z, z, int(iter), 4.0);
    } else {
        z.x = down_left.x + ((range_x*gl_FragCoord.x) - render_resolution.x * 2)/render_resolution.x;
        z.y = down_left.y + (range_y*gl_FragCoord.y)/render_resolution.y;
        mandebrot_num = mandebrot_set_degree(z, c, int(iter), 4.0);
    }

    float mandebrot = (float(mandebrot_num) /  int(iter));
    aux.x = mandebrot;
    aux.y = mandebrot;
    aux.z = mandebrot;
    color = aux;
};
