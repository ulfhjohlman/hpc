#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<CL/cl.h>

int main(int argc, char *argv[]){
    /* parse command line arguments */
    int w=3;
    int h=3;
    double i=1e6;
    int n=2;
    double d=1/30;
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
    if(argc == 6){
        w = strtol(argv[optind],(char**)NULL,10);
        h = strtol(argv[optind+1],(char**)NULL,10);
    }
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
    
    const char * opencl_program_src = 
	"__kernel void (\
	__global const int h,\
	__global const int w,\
	__global const int c,\
	__global float * old,\
	__global float * new){\
	int ix = get_global_id(0);\
	float t,d,l,r;\
	if(ix<w){\
		t=0;\
	}else{ t = old[ix-w]};\
	if(ix>= (h-1)*w){\
		d=0;\
	}else{ d = old[ix+w]};\
	if(ix % w ==0){\
		l=0;\
	}else{ l = old[ix-1]};\
	if( (ix-1)% w ==0){\
		r = 0;\
	}else{ r = old[ix+1]};\
	new[ix] = old[ix] + c*((t+d+l+r)*0.25 -old[ix]);\
	}";

    cl_program program;
    program = clCreateProgramWithSource(context,1,(const char **) &opencl_program_src, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create program with source\n");
        return 1;
    }

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
    
    const size_t size = h*w;
    cl_mem input_buffer_a, input_buffer_b;
    input_buffer_a  = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(float) * size, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create buffer a\n");
        return 1;
    }
    input_buffer_b  = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(float) * size, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create buffer b\n");
        return 1;
    }
    
    float * a = calloc(size*sizeof(float));
    float * b = calloc(size*sizeof(float));
    if(h%2 == 0){
	if(w%2==0){
		a[w*h/2+w/2] =         i*0.25;
		a[w*h/2+w/2-1] =       i*0.25;
		a[w*(h/2-1) + w/2-1] = i*0.25;
		a[w*(h/2-1) + w/2]   = i*0.25;
	}else{
		a[w*h/2+w/2] = i*0.5;
		a[w*(h/2-1)+w/2] = i*0.5;
	}
    }else{
	if(w%2==0){
		a[w*h/2 + w/2] = i*0.5;
		a[w*h/2 + w/2-1] = i*0.5;
	}else{
		a[w*h/2 + w/2] = i;
	}
    }


    error = clEnqueueWriteBuffer(command_queue, input_buffer_a, CL_TRUE,0, size*sizeof(float), a, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot enqueue buffer a \n");
        return 1;
    }
    error = clEnqueueWriteBuffer(command_queue, input_buffer_b, CL_TRUE,0, size*sizeof(float), b, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot enqueue buffer b \n");
        return 1;
    }

    error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &h);
    if(error != CL_SUCCESS) {
        printf("cannot set kernel arg h\n");
        return 1;
    }
    error = clSetKernelArg(kernel, 1, sizeof(cl_mem), &w);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg w\n");
    }
    error = clSetKernelArg(kernel, 2, sizeof(cl_mem), &d);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg d\n");
    }
    error = clSetKernelArg(kernel, 3, sizeof(cl_mem), &input_buffer_a);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg input_buffer_a");
    }
    error = clSetKernelArg(kernel, 4, sizeof(cl_mem), &input_buffer_b);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg input_buffer_b");
    }

    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,(const size_t *)&size, NULL, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot enqueue nd range kernel\n");
        return 1;
    }

    error = clEnqueueReadBuffer(command_queue, input_buffer_a, CL_TRUE,0, size*sizeof(float), a, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot read buffer a\n");
        return 1;
    }
    error = clEnqueueReadBuffer(command_queue, input_buffer_b, CL_TRUE, 0, size*sizeof(float), b, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
	printf("cannot read buffer b\n");
	return 1;
    }
    clFinish(command_queue);

    if(size <= 100)
    for(int i=0;i<w;i++){
	for(int j=0;j<h;j++){
		printf(" %f ",a[j*h+i]);
	}
	printf("\n");
    }

    free(a);
    free(b);
    clReleaseMemObject(input_buffer_a);
    clReleaseMemObject(input_buffer_b);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    return 0;
}
