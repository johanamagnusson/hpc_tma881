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
    int i, j ,k;
    float sum = 0.0;
    float average;
    float denom;

    for (k = 0; k < w * h; k++) {
        i = k % w;
        j = k / w;
        if (i == 0 || i == (w-1) || j == 0 || j == (h-1))
            ;
        else
            sum += new[k];
    }
    denom = (float) ((w-2) * (h-2));
    average = sum/denom;
    return average;
}


int main(int argc, char **argv)
{   
    int         width;
    int         height;
    int         wOff;
    int         hOff;
    float       initCentValue;
    float       diffusionConst;
    int         iterations, i, j;
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

    width  = atoi(argv[1]);
    height = atoi(argv[2]);
    wOff = width + 2;
    hOff = height + 2;

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
    float *new = (float *) calloc((width+2) * (height+2), sizeof(float));
    float *old = (float *) calloc((width+2) * (height+2), sizeof(float));

    if (height % 2 == 0) {
        old[((width+2) * (height+2) / 2) + width / 2] = initCentValue;
    } else {
        old[((width+2) * (height+2) / 2)] = initCentValue;
    }

    cl_mem buffer_new, buffer_old;
    buffer_new = clCreateBuffer(context, CL_MEM_READ_WRITE,
            (width+2) * (height+2) * sizeof(float), NULL, &error);
    if (error != CL_SUCCESS) {
        printf("cannot create buffer\n");
        return 1;
    }
    buffer_old = clCreateBuffer(context, CL_MEM_READ_WRITE,
            (width+2) * (height+2) * sizeof(float), NULL, &error);
    if (error != CL_SUCCESS) {
        printf("cannot create buffer\n");
        return 1;
    }

    error = clEnqueueWriteBuffer(command_queue, buffer_new, CL_TRUE, 0,
            (width+2) * (height+2) * sizeof(float), new, 0, NULL, NULL);
    if (error != CL_SUCCESS) {
        printf("cannot write to buffer\n");
        return 1;
    }
    error = clEnqueueWriteBuffer(command_queue, buffer_old, CL_TRUE, 0,
            (width+2) * (height+2) * sizeof(float), old, 0, NULL, NULL);
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


    kernel = clCreateKernel(program, "diffusion", &error);

    error = clSetKernelArg(kernel , 2 , sizeof(int)    , &hOff);
    if (error != CL_SUCCESS) {
        printf("cannot set argument 2 \n");
        return 1;
    }
    error = clSetKernelArg(kernel , 3 , sizeof(int)    , &wOff);
    if (error != CL_SUCCESS) {
        printf("cannot set argument 3 \n");
        return 1;
    }
    error = clSetKernelArg(kernel , 4 , sizeof(float)  , &diffusionConst);
    if (error != CL_SUCCESS) {
        printf("cannot set argument 4 \n");
        return 1;
    }

    const size_t global[] = {height, width};

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
    
        error = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL,
                (const size_t *)&global, NULL, 0, NULL, NULL);
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
                (width+2) * (height+2) * sizeof(float), new, 0, NULL, NULL);
        if (error != CL_SUCCESS) {
            printf("cannot read buffer \n");
            return 1;
        }
    } else {
        error = clEnqueueReadBuffer(command_queue, buffer_new, CL_TRUE, 0,
                (width+2) * (height+2) * sizeof(float), new, 0, NULL, NULL);
        if (error != CL_SUCCESS) {
            printf("cannot read buffer \n");
            return 1;
        }
    }

    //for (i = 0; i < wOff * hOff; i++)
    //    printf("%05.5e\n", new[i]);

    error = clFinish(command_queue);
    float average;
    float standDiv;
    
    average = aveCalc(new, wOff, hOff);    

    float diff;
    for(i = 0; i < (width+2)*(height+2); ++i){
        diff = new[i] - average;
        new[i] = fabsf(diff);
    }
    standDiv = aveCalc(new, wOff, hOff);

    printf("Average            : %05.5e\n", average);
    printf("Standard deviation : %05.5e\n", standDiv);

    clReleaseContext(context);
    clReleaseCommandQueue(command_queue);
    clReleaseMemObject(buffer_new);
    clReleaseMemObject(buffer_old);

    free(new);
    free(old);

    return 0;
}
