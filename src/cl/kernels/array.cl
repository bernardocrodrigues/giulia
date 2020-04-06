__kernel void ProcessArray(__global int* in, __global int* out)
{
    out[get_global_id(0)] = 2 * in[get_global_id(0)];
}