//
//  main.c
//  matrix_multiplication
//
//  Created by AbdelRahman AbdelFattah on 29/03/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

int a[20][20];
int b[20][20];
int c[20][20];

int check_if_compatible(int col1, int row2){
	if(col1 == -1 || row2 == -1){
		return 0;
	}
	if(col1 == row2){
		return 1;
	}
	return 0;
}

int return_value(char string[]){
    char temp[20];
    int j = 0;
    for(int i = 0; i< 256; i++){
        if(string[i]>='0' && string[i]<='9'){
            temp[j]=string[i];
            j++;
        }
        if(string[i]=='\0'){
            temp[j]='\0';
            break;
        }
    }
    return atoi(temp);
}

int main(int argc, const char * argv[]) {
	struct timeval stop, start;
	char * mat1 = malloc(64);
	char * mat2 = malloc(64);
	char * matout = malloc(64);
	int row1 = -1,col1 = -1;
	int row2 = -1,col2 = -1;
	if(argc == 4){
		strncpy(mat1, argv[1], strlen(argv[1]));
		strncpy(mat2, argv[2], strlen(argv[2]));
		strncpy(matout, argv[3], strlen(argv[3]));
	}else{
		mat1 = "a";
		mat2 = "b";
		matout = "c";
	}
	strcat(mat1, ".txt");
	strcat(mat2, ".txt");
	strcat(matout, ".txt");
    char row [256];
    char col [256];
    // File 1
    FILE *fp1 = fopen(mat1, "r");
    fscanf(fp1, "%s", row); //Scan row
    fscanf(fp1, "%s", col); //Scan col
    row1 = return_value(row);
    col1 = return_value(col);
    for(int i = 0; i<row1;i++){
        for(int j = 0; j<col1;j++){
            char * temp = malloc(sizeof(int));
            fscanf(fp1, "%s", temp);
            a[i][j] = atoi(temp);
        }
    }
    for(int i = 0; i<row1;i++){
        for(int j = 0; j<col1;j++){
            printf("%d\t", a[i][j]);
        }
        printf("\n");
    }
    fclose(fp1);
    // File 2
    FILE *fp2 = fopen(mat2, "r");
    fscanf(fp2, "%s", row); //Scan row
    fscanf(fp2, "%s", col); //Scan col
    row2 = return_value(row);
    col2 = return_value(col);
    for(int i = 0; i<row2;i++){
        for(int j = 0; j<col2;j++){
            char * temp = malloc(sizeof(int));
            fscanf(fp2, "%s", temp);
            b[i][j] = atoi(temp);
        }
    }
    for(int i = 0; i<row2;i++){
        for(int j = 0; j<col2;j++){
            printf("%d\t", b[i][j]);
        }
        printf("\n");
    }
    fclose(fp2);

	if(check_if_compatible(col1,row2)){
		gettimeofday(&start, NULL); //start checking time
			//your code goes here
		gettimeofday(&stop, NULL); //end checking time

		printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
		printf("Microseconds taken: %d\n", stop.tv_usec - start.tv_usec);
	}else{
		printf("Dimensions are not compatible\n");
	}
	return 0;
}
