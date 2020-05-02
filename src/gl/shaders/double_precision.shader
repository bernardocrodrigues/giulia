#shader vertex
#version 400 core

layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position;
};

#shader fragment
#version 400 core

layout(location = 0) out vec4 color;
uniform dvec2 down_left;
uniform double range_;
uniform uvec2 screen_resolution;

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

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}


int mandebrot_set_degree(in dvec2 candidate, in int max_steps, in float threshold) {

    dvec2 c = candidate;
    dvec2 z;
    z.x = 0.0;
    z.y = 0.0;
    int index = 0;

    while (index < max_steps && (magnetude(z) < threshold)) {
        z = add_(product(z,z), c);
        index++;
    }

    return index;
}

void main()
{
    dvec2 imaginary_number;
    imaginary_number.x = down_left.x + (range_*gl_FragCoord.x)/screen_resolution.x;
    imaginary_number.y = down_left.y + (range_*gl_FragCoord.y)/screen_resolution.y;

    int mandebrot_num = mandebrot_set_degree(imaginary_number, 256, 4.0);
    float mandebrot = (float(mandebrot_num) / 256);

    vec4 aux;
    aux.x = mandebrot;
    aux.y = mandebrot;
    aux.z = mandebrot;
    color = aux;
};
