#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <math.h>
#include <CL/cl.h>


struct arguments
{
    long double i;
    double d;
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

int main(int argc, char **argv)
{   
    int         width;
    int         height;
    long double initCentValue;
    double      diffusionConst;
    int         iterations;
    struct      arguments arguments;

    /* OpenCL requirements */
    cl_int              error;
    cl_platform_id      platform_id;
    cl_uint             nmb_platforms;
    cl_device_id        device_id;
    cl_uint             nmb_devices;
    cl_context          context;
    cl_command_queue    command_queue;

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

    argp_parse (&argp, argc, argv, 0, 0, &arguments); 

    initCentValue  = arguments.i;
    diffusionConst = arguments.d;
    iterations     = arguments.n;
    
    printf("Width                 : %d\n", width);
    printf("Height                : %d\n", height);
    printf("Initial central value : %Le\n", initCentValue);
    printf("Diffusion constant    : %.2f\n", diffusionConst);
    printf("Number of iterations  : %d\n", iterations);
    
    clReleaseContext(context);
    clReleaseCommandQueue(command_queue);

    return 0;
}
