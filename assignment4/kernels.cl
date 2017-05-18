__kernel void diffusion(__global float* old_temp, __global float* new_temp, int h, int l, float diff_const)
{
    int i = get_global_id(0) + 1;
    int j = get_global_id(1) + 1;
    float element = 0;

    element += old_temp[(i+1)*l + j] + old_temp[(i-1)*l + j];
    element += old_temp[i*l + j+1] + old_temp[i*l + j-1];
    
    new_temp[i*l + j] = old_temp[i*l + j] + diff_const * (element/4 - old_temp[i*l + j]);
}

