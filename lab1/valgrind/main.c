#include<stdio.h>
#include<stdlib.h>

void allocMemmory(){
	int *x;
	x = malloc(1000 * sizeof(*x));
}

int main(){
	allocMemmory();
	unsigned long y = 0;
	unsigned long i;
	for(i =1; i <= 1000000000; i++){
		y += i;
	}
}
