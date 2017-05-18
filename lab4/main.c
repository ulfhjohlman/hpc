#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<CL/cl.h>

int main(int argc, char *argv[]){
    /* parse command line arguments */
    int w=10;
    int h=10;
    double i=100;
    int n=1;
    double d=0.1;
    int opt;
    while ((opt = getopt(argc, argv, "i:n:d:")) != -1) {
        switch (opt) {
            case 'i': i = (double) strtod(optarg,(char**)NULL); break;
            case 'n': n = (int) strtol(optarg,(char**)NULL,10); break;
            case 'd': d = (double) strtod(optarg,(char**)NULL); break;
            default:
                fprintf(stderr, "Usage: %s [width] [height] [-i][centerValue] [-d][diffusionConstant] [-n][nIterations]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if(argc != 6){
        printf("wrong number of arguments\n");
    }
    int index;
    w = strtol(argv[optind],(char**)NULL,10);
    h = strtol(argv[optind+1],(char**)NULL,10);
    printf("w: %d, h: %d i:%fl n: %d, d :%lf\n",w,h,i,n,d);
    
    cl_int error;
    cl_platform_id platform_id;
    cl_uint nmb_platforms;
    if (clGetPlatformIDs(1, &platform_id, &nmb_platforms) != CL_SUCCESS) {
         printf( "cannot get platform\n" );
         return 1;
    }
    cl_device_id device_id;
    cl_uint nmb_devices;
    if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &nmb_devices) != CL_SUCCESS) {
         printf( "cannot get device\n" );
         return 1;
    }
    cl_context context;
    cl_context_properties properties[] ={CL_CONTEXT_PLATFORM,(cl_context_properties) platform_id,0};
    context = clCreateContext(properties, 1, &device_id, NULL, NULL, &error);

    cl_command_queue command_queue;
    command_queue = clCreateCommandQueue(context, device_id, 0, &error);
    if (error != CL_SUCCESS) {
        printf("cannot create context\n");
        return 1;
    }
    
    //const char * opencl_program_src = "__kernel void dot_prod_mul(__global const float * a,__global const float * b,__global float * c){int ix = get_global_id(0);c[ix] = a[ix] * b[ix];}";

    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    if (f)
    {
	fseek (f, 0, SEEK_END);
	length = ftell (f);
	fseek (f, 0, SEEK_SET);
	buffer = malloc (length);
	if (buffer)
	{
	    fread (buffer, 1, length, f);
	}
	fclose (f);
    }
    const char * opencl_program_src = buffer;

    cl_program program;
    program = clCreateProgramWithSource(context,1,(const char **) &opencl_program_src, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create program with source\n");
        return 1;
    }

    //clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    error = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (error != CL_SUCCESS) {
        printf("cannot build program.\n Source: %s\n log:\n",opencl_program_src);
	size_t log_size = 0;
  	clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,0, NULL, &log_size);
	char * log = calloc(log_size, sizeof(char));
	if (log == NULL) {
		printf("could not allocate memory\n");
		return 1;
	}
	clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
	log_size, log, NULL);
	printf( "%s\n", log );
	free(log);
	return 1;
    }
    
    cl_kernel kernel;
    kernel = clCreateKernel(program, "dot_prod_mul", &error);
    if(error != CL_SUCCESS) {
        printf("cannot create Kernel\n");
        return 1;
    }
    


    const size_t ix_m = 10e7;
    cl_mem input_buffer_a, input_buffer_b, output_buffer_c;
    input_buffer_a  = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(float) * ix_m, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create buffer a\n");
        return 1;
    }
    input_buffer_b  = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(float) * ix_m, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create buffer b\n");
        return 1;
    }
    output_buffer_c = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(float) * ix_m, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create buffer c\n");
        return 1;
    }

    float * a = malloc(ix_m*sizeof(float));
    float * b = malloc(ix_m*sizeof(float));
    for (size_t ix; ix < ix_m; ++ix) {
	a[ix] = ix;
	b[ix] = ix;
    }
    error = clEnqueueWriteBuffer(command_queue, input_buffer_a, CL_TRUE,0, ix_m*sizeof(float), a, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot enqueue buffer a c\n");
        return 1;
    }
    error = clEnqueueWriteBuffer(command_queue, input_buffer_b, CL_TRUE,0, ix_m*sizeof(float), b, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot enqueue buffer b c\n");
        return 1;
    }

    error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer_a);
    if(error != CL_SUCCESS) {
        printf("cannot set kernel arg a\n");
        return 1;
    }
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &input_buffer_b);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &output_buffer_c);

    const size_t global = ix_m;
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,(const size_t *)&global, NULL, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot enqueue nd range kernel\n");
        return 1;
    }

    float * c = malloc(ix_m*sizeof(float));
    if(c = NULL){
         printf("cannot malloc c\n");
         return 1;
    }
    error = clEnqueueReadBuffer(command_queue, output_buffer_c, CL_TRUE,0, ix_m*sizeof(float), c, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot set kernel arg a\n");
        return 1;
    }
    clFinish(command_queue);

    printf("Square of %d is %f\n", 100, c[100]);

    free(a);
    free(b);
    free(c);

    clReleaseMemObject(input_buffer_a);
    clReleaseMemObject(input_buffer_b);
    clReleaseMemObject(output_buffer_c);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    return 0;
}
