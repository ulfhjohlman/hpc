#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<CL/cl.h>
#include<math.h>

int main(int argc, char *argv[]){
    /* parse command line arguments */
    int w=3;
    int h=3;
    float i=1e6;
    int n=2;
    float d=(1.0/30);
    int opt;
    while ((opt = getopt(argc, argv, "i:n:d:")) != -1) {
        switch (opt) {
            case 'i': i = (float) strtof(optarg,(char**)NULL); break;
            case 'n': n = (int) strtol(optarg,(char**)NULL,10); break;
            case 'd': d = (float) strtof(optarg,(char**)NULL); break;
            default:
                fprintf(stderr, "Usage: %s [width] [height] [-i][centerValue] [-d][diffusionConstant] [-n][nIterations]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if(argc != 6){
        printf("wrong number of arguments\n");
    }
    if(n%2!= 0){
	printf("n must be even!\n");
	return 1;
    }
    int index;
    if(argc == 6){
        w = strtol(argv[optind],(char**)NULL,10);
        h = strtol(argv[optind+1],(char**)NULL,10);
    }
   // printf("w: %d, h: %d i:%f n: %d, d :%f\n",w,h,i,n,d);
    
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
	"__kernel void diffuse(\
	int h,\
	int w,\
	float c,\
	__global const float * old,\
	__global float * new){\
	int ix = get_global_id(0);\
	float t,d,l,r;\
	if(ix<w){\
		t=0;\
	}else{ t = old[ix-w];}\
	if(ix>= (h-1)* w){\
		d=0;\
	}else{ d = old[ix+ w];}\
	if(ix % w ==0){\
		l=0;\
	}else{ l = old[ix-1];}\
	if( (ix+1)% w ==0){\
		r = 0;\
	}else{ r = old[ix+1];}\
	new[ix] = old[ix]+ c*((t+d+l+r)*0.25 - old[ix]);\
	}";

    cl_program program;
    program = clCreateProgramWithSource(context,1,(const char **) &opencl_program_src, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create program with source\n");
        return 1;
    }

    error = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (error != CL_SUCCESS) {
        printf("cannot build program.\n log:\n");
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
    kernel = clCreateKernel(program, "diffuse", &error);
    if(error != CL_SUCCESS) {
        printf("cannot create Kernel\n");
        return 1;
    }
    
    const size_t size = h*w;
    cl_mem input_buffer_a, input_buffer_b, input_buffer_h, input_buffer_w, input_buffer_d;
    input_buffer_a  = clCreateBuffer(context, CL_MEM_READ_WRITE,sizeof(float) * size, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create buffer a\n");
        return 1;
    }
    input_buffer_b  = clCreateBuffer(context, CL_MEM_READ_WRITE,sizeof(float) * size, NULL, &error);
    if(error != CL_SUCCESS) {
        printf("cannot create buffer b\n");
        return 1;
    }
//    input_buffer_h  = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(int) * size, NULL, &error);
//    input_buffer_w  = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(int) * size, NULL, &error);
//    input_buffer_d  = clCreateBuffer(context, CL_MEM_READ_ONLY,sizeof(int) * size, NULL, &error);
   
    float * a = (float *) calloc(size,sizeof(float));
    float * b = (float *) calloc(size,sizeof(float));
    if(h%2 == 0){
	if(w%2==0){
		a[w*(h/2)+w/2] =      i*0.25;
		a[w*(h/2)+w/2-1] =    i*0.25;
		a[w*(h/2-1) + w/2-1] =i*0.25;
		a[w*(h/2-1) + w/2] =  i*0.25;
	}else{
		printf("h & w must both be even or odd\n");
		return 1;
	}
    }else{
	if(w%2==0){
		printf("h & w must both be even or odd\n");
		return 1;
	}else{
		a[w*(h/2) + w/2] =  i;
	}
    }
   /* if(size<=100)
    for (int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			printf(" %f ",a[i*h+j]);
		} printf("\n");
    }
   */ 

    
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
//    error = clEnqueueWriteBuffer(command_queue, input_buffer_h, CL_TRUE, 0, size*sizeof(int), &h, 0 ,NULL, NULL);
//    if(error != CL_SUCCESS) {
//	printf("cannot enqueue buffer h\n");
//	return(1);
//    }
//    error = clEnqueueWriteBuffer(command_queue, input_buffer_w, CL_TRUE, 0, size*sizeof(int), &w, 0 ,NULL, NULL);
//    error = clEnqueueWriteBuffer(command_queue, input_buffer_d, CL_TRUE, 0, size*sizeof(int), &d, 0 ,NULL, NULL);

//printf("setting constant args:\n");
    
    error = clSetKernelArg(kernel, 0, sizeof(int), &h);
    if(error != CL_SUCCESS) {
        printf("cannot set kernel arg h\n");
        return 1;
    }
    error = clSetKernelArg(kernel, 1, sizeof(int), &w);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg w\n");
    }
    error = clSetKernelArg(kernel, 2, sizeof(float), &d);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg d\n");
    }

for(int i=0;i<n/2;i++){
//printf("setting buffer args:\n");
    error = clSetKernelArg(kernel, 3, sizeof(cl_mem), &input_buffer_a);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg input_buffer_a\n");
    }
    error = clSetKernelArg(kernel, 4, sizeof(cl_mem), &input_buffer_b);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg input_buffer_b\n");
    }
//printf("Enquing:\n");
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,(const size_t *)&size, NULL, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot enqueue nd range kernel 1\n");
        return 1;
    }
    
    error = clSetKernelArg(kernel, 4, sizeof(cl_mem), &input_buffer_a);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg input_buffer_a 2 \n");
        return 1;
    }

    error = clSetKernelArg(kernel, 3, sizeof(cl_mem), &input_buffer_b);
    if(error != CL_SUCCESS) {
	printf("cannot set kernel arg input_buffer_b 2 \n");
        return 1;
    }

   
    error = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,(const size_t *)&size, NULL, 0, NULL, NULL);
    if(error != CL_SUCCESS) {
        printf("cannot enqueue nd range kernel 2\n");
        return 1;
    }
}


//printf("reading:\n");
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
    
    if(size <= 100){
    printf("Printing a:\n");
    for(int i=0;i<h;i++){
	for(int j=0;j<w;j++){
		printf(" %f ",a[i*h+j]);
	}
	printf("\n");
    }}

    if(size <= 100){
    printf("Printing b:\n");
    for(int i=0;i<h;i++){
	for(int j=0;j<w;j++){
		printf(" %f ",b[i*h+j]);
	}
	printf("\n");
        }
    }
    double avg_tmp=0;
    for(int i =0;i < size; i++){
	avg_tmp += a[i]/size;
    }
    //avg_tmp = avg_tmp/size;
    printf("Average temp: %lf\n",avg_tmp);

    double abs_diff_avg_tmp=0;
    for(int i =0; i<size;i++){
	abs_diff_avg_tmp += fabsf(a[i]-avg_tmp)/size;
	//printf("%f\n",fabsf(a[i]-avg_tmp));
    }
//    abs_diff_avg_tmp= abs_diff_avg_tmp/size;
    printf("Average abs diff from average tmp: %lf\n",abs_diff_avg_tmp);

    free(a);
    free(b);
    clReleaseMemObject(input_buffer_a);
    clReleaseMemObject(input_buffer_b);
    clReleaseMemObject(input_buffer_h);
    clReleaseMemObject(input_buffer_w);
    clReleaseMemObject(input_buffer_d);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    return 0;
}
