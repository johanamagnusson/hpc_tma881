__kernel void diffusion(__global float* old_temp, __global float* new_temp, __global int height, __global int length, __global float diff_const)
{
    int i = get_global_id(0);
    int j = get_global_id(1);
    float element = 0.;

    if(i == 0){
        element += 2*old_temp[(i+1)*length + j];
    }elseif(i == height-1){
        element += old_temp[(i-1)*length + j];
    }else{
        element += old_temp[(i+1)*length + j] + old_temp[(i-1)*legnth + j];
    }

    if(j == 0){
        element += 2*old_temp[i*length + j+1];
    }elseif(j == length-1){
        element += old_temp[i*legnth + j-1];
    }else{
        element += old_temp[i*length + j+1] + old_temp[i*length + j-1];
    }
    
    new_temp[i*length + j] = old_temp[i*length + j] + diff_const * (element/4 - old_temp[i*length + j]);
}
