#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <math.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

struct arguments
{
    float i;
    float d;
    int n;
};

static int
parse_opt(int key, char *arg,
        struct argp_state *state)
{
    struct arguments *arguments = state -> input;
    switch (key)
    {
        case 'i':
            {
                arguments -> i = atof(arg);
                break;
            }
        case 'd':
            {
                arguments -> d = atof(arg);
                break;
            }
        case 'n':
            {
                arguments -> n = atoi(arg);
                break;
            }
    }
    return 0;
}

struct argp_option options[] =
{
    {"initial central value" , 'i' , "NUM" , 0 , "The mid point starting value"}, 
    {"diffusion constant"    , 'd' , "NUM" , 0 , "The diffusion constant"}, 
    {"iterations"            , 'n' , "NUM" , 0 , "The number of iterations"}, 
    {0}
};

struct argp argp = { options, parse_opt, 0, 0 };

float aveCalc(float *new, int w, int h)
{
    int i;
    float sum = 0.0;
    float average;
    int halfw = (int) ((float) w/2.0 + 0.6);
    int halfh = (int) ((float) h/2.0 + 0.6);
    float toMuch1 = 0.0;
    float toMuch2 = 0.0;
    float denom;
    
    if((w%2 == 0) && (h%2 == 0)){
        for(i = 0; i < halfw*halfh ; i++){
            sum += new[i];
        }
        sum = sum*4.0;
    }else if((w%2 == 0) && (h%2 == 1)){
        for(i = 0; i < halfw*halfh ; i++){
            sum += new[i];
        }
        for(i = 0; i < halfw; i++){
            toMuch1 += new[i];
        }
        sum = sum*4.0;
        sum = sum - 2.0*toMuch1;
    }else if((w%2 == 1) && (h%2 == 0)){
        for(i = 0; i < halfw*halfh ; i++){
            sum += new[i];
        }
        for(i = 0; i < halfh; i++){
            toMuch1 += new[i*halfw];
        }
        sum = sum*4.0;
        sum = sum - 2.0*toMuch1;
    }else{
        for(i = 0; i < halfw*halfh ; i++){
            sum += new[i];
        }
        for(i = 0; i < halfw; i++){
            toMuch1 += new[i];
        }
        for(i = 0; i < halfh; i++){
            toMuch2 += new[i*halfw];
        }
        sum = sum*4.0;
        sum = sum - 2.0*toMuch1 - 2.0*toMuch2 + new[0];
    }
    denom = (float) (w * h);
    average = sum/denom;
    return average;
}



int main(int argc, char **argv)
{   
    int         width;
    int         height;
    int         fullWidth;
    int         fullHeight;
    float       initCentValue;
    float       diffusionConst;
    int         iterations, i;
    struct      arguments arguments;

    /* OpenCL requirements */
    cl_int              error;
    cl_platform_id      platform_id;
    cl_uint             nmb_platforms;
    cl_device_id        device_id;
    cl_uint             nmb_devices;
    cl_context          context;
    cl_command_queue    command_queue;
    cl_program          program;
    cl_kernel           kernel;

    if (clGetPlatformIDs(1, &platform_id, &nmb_platforms) != CL_SUCCESS)
    {
        printf("Cannot get platform\n");
        return 1;
    }
    if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU,
                1, &device_id, &nmb_devices) != CL_SUCCESS)
    {
        printf("Cannot get device\n");
        return 1;
    }
    cl_context_properties properties[] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties) platform_id,
        0
    };
    context = clCreateContext(properties, 1, &device_id, NULL, NULL, &error);
    if (error != CL_SUCCESS)
    {
        printf("Cannot create context\n");
        return 1;
    }
    command_queue = clCreateCommandQueue(context, device_id, 0, &error);
    if (error != CL_SUCCESS)
    {
        printf("Cannot create queue\n");
        return 1;
    }

    fullWidth  = atoi(argv[1]);
    fullHeight = atoi(argv[2]);
    width = (int) ((float) fullWidth / 2.0 + 0.6);
    height = (int) ((float) fullHeight / 2.0 + 0.6);

    argp_parse (&argp, argc, argv, 0, 0, &arguments); 

    initCentValue  = arguments.i;
    diffusionConst = arguments.d;
    iterations     = arguments.n;
    
    /*
    printf("Width                 : %d\n", width);
    printf("Height                : %d\n", height);
    printf("Initial central value : %le\n", initCentValue);
    printf("Diffusion constant    : %.2f\n", diffusionConst);
    printf("Number of iterations  : %d\n", iterations);
    */
    /* Create buffers and allocate memory */
    float *new = (float *) calloc(width * height, sizeof(float));
    float *old = (float *) calloc(width * height, sizeof(float));

    char *kernelString;
    if (fullWidth % 2 == 0 && fullHeight % 2 == 0) {
        kernelString = "diffusion_both_even";
        old[0] = initCentValue / 4.0;
    } else if (fullWidth % 2 == 0 && fullHeight % 2 != 0) {
        kernelString = "diffusion_height_uneven_length_even";
        old[0] = initCentValue / 2.0;
    } else if (fullWidth % 2 != 0 && fullHeight % 2 == 0) {
        kernelString = "diffusion_height_even_length_uneven";
        old[0] = initCentValue / 2.0;
    } else {
        kernelString = "diffusion_both_uneven";
        old[0] = initCentValue;
    }


    cl_mem buffer_new, buffer_old;
    buffer_new = clCreateBuffer(context, CL_MEM_READ_WRITE,
            width * height * sizeof(float), NULL, &error);
    if (error != CL_SUCCESS) {
        printf("cannot create buffer\n");
        return 1;
    }
    buffer_old = clCreateBuffer(context, CL_MEM_READ_WRITE,
            width * height * sizeof(float), NULL, &error);
    if (error != CL_SUCCESS) {
        printf("cannot create buffer\n");
        return 1;
    }

    error = clEnqueueWriteBuffer(command_queue, buffer_new, CL_TRUE, 0,
            width * height * sizeof(float), new, 0, NULL, NULL);
    if (error != CL_SUCCESS) {
        printf("cannot write to buffer\n");
        return 1;
    }
    error = clEnqueueWriteBuffer(command_queue, buffer_old, CL_TRUE, 0,
            width * height * sizeof(float), old, 0, NULL, NULL);
    if (error != CL_SUCCESS) {
        printf("cannot write to buffer \n");
        return 1;
    }

    /* Make program and execute */
    FILE *fp;
    const char fileName[] = "./kernels.cl";
    size_t source_size;
    char *source_str;

    fp = fopen(fileName, "r");
        if (!fp)
        {
            fprintf(stderr, "Failed to load kernel.\n");    
            exit(1);
        }    
    source_str = (char *)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    program = clCreateProgramWithSource(context, 1, (const char **) &source_str,
            (const size_t *) &source_size, &error);
    if (error != CL_SUCCESS) {
        printf("cannot create program\n");
        return 1;
    }
    error = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (error != CL_SUCCESS) {
        printf("cannot compile program \n");
        return 1;
    }


    kernel = clCreateKernel(program, kernelString, &error);

    error = clSetKernelArg(kernel , 0 , sizeof(cl_mem) , (void *)&buffer_old);
    if (error != CL_SUCCESS) {
        printf("cannot set argument 0 \n");
        return 1;
    }
    error = clSetKernelArg(kernel , 1 , sizeof(cl_mem) , (void *)&buffer_new);
    if (error != CL_SUCCESS) {
        printf("cannot set argument 1 \n");
        return 1;
    }
    error = clSetKernelArg(kernel , 2 , sizeof(int)    , &height);
    if (error != CL_SUCCESS) {
        printf("cannot set argument 2 \n");
        return 1;
    }
    error = clSetKernelArg(kernel , 3 , sizeof(int)    , &width);
    if (error != CL_SUCCESS) {
        printf("cannot set argument 3 \n");
        return 1;
    }
    error = clSetKernelArg(kernel , 4 , sizeof(float)  , &diffusionConst);
    if (error != CL_SUCCESS) {
        printf("cannot set argument 4 \n");
        return 1;
    }
    
    //size_t global_item_size = width * height;
    //size_t local_item_size = 1;

    const size_t global[] = {width, height}

    for (i = 0; i < iterations; i++)
    {
        error = clSetKernelArg(kernel , i % 2, sizeof(cl_mem) , (void *)&buffer_old);
        if (error != CL_SUCCESS) {
            printf("cannot set argument 0 \n");
            return 1;
        }
        error = clSetKernelArg(kernel , (i+1) % 2 , sizeof(cl_mem) , (void *)&buffer_new);
        if (error != CL_SUCCESS) {
            printf("cannot set argument 1 \n");
            return 1;
        }
    
        //error = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL,
        //        &global_item_size, &local_item_size, 0, NULL, NULL);
        error = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL,
                (const size_t *)&global, 0, NULL, NULL);
        if (error != CL_SUCCESS) {
            printf("cannot run kernel \n");
            return 1;
        }

        error = clFinish(command_queue);
        if (error != CL_SUCCESS) {
            printf("Finish error \n");
            return 1;
        }

    }
    
    if (i % 2 == 0) {
        error = clEnqueueReadBuffer(command_queue, buffer_old, CL_TRUE, 0,
                width * height * sizeof(float), new, 0, NULL, NULL);
        if (error != CL_SUCCESS) {
            printf("cannot read buffer \n");
            return 1;
        }
    } else {
        error = clEnqueueReadBuffer(command_queue, buffer_new, CL_TRUE, 0,
                width * height * sizeof(float), new, 0, NULL, NULL);
        if (error != CL_SUCCESS) {
            printf("cannot read buffer \n");
            return 1;
        }
    }

    error = clFinish(command_queue);
    /*
    float average;
    float standDiv;
    
    average = aveCalc(new, fullWidth, fullHeight);    

    float diff;
    for(i = 0; i < width*height; ++i){
        diff = new[i] - average;
        //printf("%05.2f\n", diff);
        new[i] = fabsf(diff);
    }
    standDiv = aveCalc(new, fullWidth, fullHeight);

    printf("Average            : %05.2e\n", average);
    printf("Standard deviation : %05.2e\n", standDiv);
    */

    clReleaseContext(context);
    clReleaseCommandQueue(command_queue);
    clReleaseMemObject(buffer_new);
    clReleaseMemObject(buffer_old);

    free(new);
    free(old);

    return 0;
}
