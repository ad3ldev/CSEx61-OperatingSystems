//
//  main.c
//  matrix_multiplication
//
//  Created by AbdelRahman AbdelFattah on 29/03/2022.
//

#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

int a[20][20];
int b[20][20];
int c[20][20];

int check_if_compatible(int col1, int row2){
	if(col1 == NULL || row2 == NULL){
		return 0;
	}
	if(col1 == row2){
		return 1;
	}
	return 0;
}

int main(int argc, const char * argv[]) {
	struct timeval stop, start;
	char * mat1;
	char * mat2;
	char * matout;
	if(argc == 4){
		mat1 = argv[1];
		mat2 = argv[2];
		matout = argv[3];
	}else{
		mat1 = "a";
		mat2 = "b";
		matout = "c";
	}
	strcat(mat1, ".txt");
    strcat(mat2, ".txt");
    strcat(matout, ".txt");
	printf("%s\n", mat1);
	int row1,col1;
	int row2,col2;
	if(check_if_compatible(col1,row2)){
		gettimeofday(&start, NULL); //start checking time
			//your code goes here
		gettimeofday(&stop, NULL); //end checking time

		printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
		printf("Microseconds taken: %d\n", stop.tv_usec - start.tv_usec);
	}else{
		printf("Dimensions are not compatiable\n");
	}
	return 0;
}
