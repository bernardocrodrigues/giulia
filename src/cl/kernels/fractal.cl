kernel void Fractal(write_only image2d_t out)
{

    const int gx = get_global_id(0);
    const int gy = get_global_id(1);

    float4 color = {8.7058824e-01f,     8.7058824e-01f,     8.7058824e-01f,     1.0000};

    write_imagef(out, (int2)(gx,gy), color);


}