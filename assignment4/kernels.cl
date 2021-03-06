__kernel void diffusion(__global float* old_temp, __global float* new_temp, int l, float diff_const)
{
<<<<<<< HEAD
    int i = get_global_id(0);
    int j = get_global_id(1);
    float element = 0;

    if(i == 0){
        element += 2*old_temp[(i+1)*l + j];
    }else if(i == h-1){
        element += old_temp[(i-1)*l + j];
    }else{
        element += old_temp[(i+1)*l + j] + old_temp[(i-1)*l + j];
    }

    if(j == 0){
        element += 2*old_temp[i*l + j+1];
    }else if(j == l-1){
        element += old_temp[i*l + j-1];
    }else{
        element += old_temp[i*l + j+1] + old_temp[i*l + j-1];
    }
    
    new_temp[i*l + j] = old_temp[i*l + j] + diff_const * (element*0.25 - old_temp[i*l + j]);
}

__kernel void diffusion_both_even(__global float* old_temp, __global float* new_temp, int h, int l, float diff_const)
{
    int i = get_global_id(0);
    int j = get_global_id(1);
    float element = 0;
    int itl = i*l;

    if(i == 0){
        element += old_temp[itl + j] + old_temp[itl+l + j];
    }else if(i == h-1){
        element += old_temp[itl-l + j];
    }else{
        element += old_temp[itl+l + j] + old_temp[itl-l + j];
    }

    if(j == 0){
        element += old_temp[itl + j] + old_temp[itl + j+1];
    }else if(j == l-1){
        element += old_temp[itl + j-1];
    }else{
        element += old_temp[itl + j+1] + old_temp[itl + j-1];
    }
    
    new_temp[itl + j] = old_temp[itl + j] + diff_const * (element*0.25 - old_temp[itl + j]);
}
__kernel void diffusion_height_uneven_lenght_eaven(__global float* old_temp, __global float* new_temp, int h, int l, float diff_const)
{
    int i = get_global_id(0);
    int j = get_global_id(1);
    float element = 0;

    if(i == 0){
        element += old_temp[i*l + j] + old_temp[(i+1)*l + j];
    }else if(i == h-1){
        element += old_temp[(i-1)*l + j];
    }else{
        element += old_temp[(i+1)*l + j] + old_temp[(i-1)*l + j];
    }

    if(j == 0){
        element += 2*old_temp[i*l + j+1];
    }else if(j == l-1){
        element += old_temp[i*l + j-1];
    }else{
        element += old_temp[i*l + j+1] + old_temp[i*l + j-1];
    }
    
    new_temp[i*l + j] = old_temp[i*l + j] + diff_const * (element*0.25 - old_temp[i*l + j]);
}



__kernel void diffusion_height_even_length_uneaven(__global float* old_temp, __global float* new_temp, int h, int l, float diff_const)
{
    int i = get_global_id(0);
    int j = get_global_id(1);
    float element = 0;

    if(i == 0){
        element += 2*old_temp[(i+1)*l + j];
    }else if(i == h-1){
        element += old_temp[(i-1)*l + j];
    }else{
        element += old_temp[(i+1)*l + j] + old_temp[(i-1)*l + j];
    }

    if(j == 0){
        element += old_temp[i*l + j] + old_temp[i*l + j+1];
    }else if(j == l-1){
        element += old_temp[i*l + j-1];
    }else{
        element += old_temp[i*l + j+1] + old_temp[i*l + j-1];
    }
    
    new_temp[i*l + j] = old_temp[i*l + j] + diff_const * (element*0.25 - old_temp[i*l + j]);
}
=======
    int i = get_global_id(0) + 1;
    int j = get_global_id(1) + 1;
    float element = 0;
    int itl = i*l;

    element += old_temp[itl + j+1] + old_temp[itl + j-1] + old_temp[itl+l + j] + old_temp[itl-l + j];
    
    new_temp[itl + j] = old_temp[itl + j] + diff_const * (element * 0.25 - old_temp[itl + j]);
}

>>>>>>> alter
