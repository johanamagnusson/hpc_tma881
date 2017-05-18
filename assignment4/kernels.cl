__kernel void diffusion(__global float* old_temp, __global float* new_temp, int h, int l, float diff_const)
{
    int i = get_global_id(0) + 1;
    int j = get_global_id(1) + 1;
    float element = 0;
    int itl = i*l;

    element += old_temp[itl+l + j] + old_temp[itl-l + j];
    element += old_temp[itl + j+1] + old_temp[itl + j-1];
    
    new_temp[itl + j] = old_temp[itl + j] + diff_const * (element/4.0 - old_temp[itl + j]);
}

