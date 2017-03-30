#include<stdio.h>
/*#include<time.h>*/

int main(void)
{
	unsigned long sum = 0;
	unsigned long i;
	/*clock_t start = clock();
	printf("Start time: %d\n", start);*/
	for(i = 1; i <= 1000000000 ; i = i + 1){
		sum += i;
	}
	/*printf("The sum of the first billion integers is: %lu\n", sum);
	clock_t end = clock();
	printf("End time: %d\n", end);
	float diff = (end-start);
	printf("Time diff (ms): %f\n", diff/CLOCKS_PER_SEC*1000);*/
	return sum;
}
