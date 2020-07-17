kernel void Fractal(write_only image2d_t out)
{

    const int gx = get_global_id(0);
    const int gy = get_global_id(1);

    float a = (float)gx;

    float4 color = {1.0000,     a,     a,     1.0000};

    write_imagef(out, (int2)(gx, gy), color);
}