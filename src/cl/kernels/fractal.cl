
float magnetude(float2 c_num){
    return sqrt((c_num.x * c_num.x) + (c_num.y * c_num.y));
}

float2 product(float2 c_num_a, float2 c_num_b) {
    float2 aux;
    aux.x = (c_num_a.x * c_num_b.x) - (c_num_a.y * c_num_b.y);
    aux.y = (c_num_a.x * c_num_b.y) + (c_num_a.y * c_num_b.x);
    return aux;
}

float2 add_(float2 c_num_a, float2 c_num_b) {
    float2 aux;
    aux.x = c_num_a.x + c_num_b.x;
    aux.y = c_num_a.y + c_num_b.y;
    return aux;
}

int mandebrot_set_degree(float2 z, float2 c, int max_steps, float threshold, uint exponent) {

    int index = 0;
    while (index < max_steps && (magnetude(z) < threshold)) {
        for(int i = 1; i < exponent; i++){
            z = product(z,z);
        }
        z = add_(z, c);
        index++;
    }
    return index;
}


kernel void Fractal(write_only image2d_t out, int mode, float position_real, float position_imaginary, float c_real, float c_imaginary, int iter, double range_x, int exponent) {

    const int gx = get_global_id(0);
    const int gy = get_global_id(1);

    int mandebrot_num;
    float2 normalized_coord;
    float2 z;
    float2 c;

    z.x = position_real + (range_x * (float) gx / 1000.0);
    z.y = position_imaginary + (range_x * (float) gy / 1000);

    c.x = c_real;
    c.y = c_imaginary;

    if (mode) {
        mandebrot_num = mandebrot_set_degree(z, c, iter, 4.0, exponent);
    } else {
        mandebrot_num = mandebrot_set_degree(z, z, iter, 4.0, exponent);
    }

    float mandebrot = ((float)(mandebrot_num) / (float)(iter));
    float4 color = {mandebrot,     mandebrot,     mandebrot,     1.0000};
    write_imagef(out, (int2)(gx, gy), color);
}


