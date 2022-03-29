//
//  main.c
//  matrix_multiplication
//
//  Created by AbdelRahman AbdelFattah on 29/03/2022.
//

#include <stdio.h>
#include <sys/time.h>

int main(int argc, const char * argv[]) {
	struct timeval stop, start;

	gettimeofday(&start, NULL); //start checking time
		//your code goes here
	gettimeofday(&stop, NULL); //end checking time

	printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
	printf("Microseconds taken: %d\n", stop.tv_usec - start.tv_usec);
	return 0;
}