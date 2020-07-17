
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


kernel void Fractal(write_only image2d_t out) {

    const int gx = get_global_id(0);
    const int gy = get_global_id(1);

    int mandebrot_num;
    float2 normalized_coord;
    float2 z;

    // normalized_coord.x = gx - 3;
    // normalized_coord.x /= 1000 * 1.024;
    // normalized_coord.y = gy / 1000 * 1.024;

    z.x = -2.5 + (3.0 * (float) gx / 1000.0);
    z.y = -1.5 + (3.0 * (float) gy / 1000);

    mandebrot_num = mandebrot_set_degree(z, z, 64, 4.0, 2);

    float mandebrot = ((float)(mandebrot_num) / (float)(64));


    // float a = (float)gx/1000;

    float4 color = {mandebrot,     mandebrot,     mandebrot,     1.0000};
    // float4 color = {1.0000,     1.0000,     1.0000,     1.0000};

    write_imagef(out, (int2)(gx, gy), color);
}


