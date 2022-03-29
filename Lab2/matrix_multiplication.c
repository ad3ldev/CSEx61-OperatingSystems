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

typedef struct Element{
    int row;
    int col;
}element;


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

void read_file(char * filename, int * row, int * col, int array[20][20]){
    char r [256];
    char c [256];
    FILE *fp = fopen(filename, "r");
    fscanf(fp, "%s", r); //Scan row
    fscanf(fp, "%s", c); //Scan col
    * row = return_value(r);
    * col = return_value(c);
    for(int i = 0; i<*row;i++){
        for(int j = 0; j<*col;j++){
            char * temp = malloc(sizeof(int));
            fscanf(fp, "%s", temp);
            array[i][j] = atoi(temp);
        }
    }
    fclose(fp);
}

void per_matrix(int x, int y, int z){
    for(int i = 0; i<x;i++){
        for(int j = 0; j<y; j++){
            c[i][j] = 0;
            for(int k = 0; k<z; k++){
                c[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
}

void per_row(int row, int y, int z){
    for(int j = 0; j<y; j++){
        c[row][j] = 0;
        for(int k = 0; k<z; k++){
            c[row][j]+=a[row][k]*b[k][j];
        }
    }
}

void per_element(element * e, int z){
    c[e->row][e->col] = 0;
    for(int k = 0; k<z; k++){
        c[e->row][e->col]+=a[e->row][k]*b[k][e->col];
    }
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
    read_file(mat1, &row1, &col1, a);
    read_file(mat2, &row2, &col2, b);
    free(mat1);
    free(mat2);
	if(check_if_compatible(col1,row2)){

        // PER MATRIX
		gettimeofday(&start, NULL); //start checking time
        printf("\nPer Matrix:\n");
		per_matrix(row1, col2, col1);
        gettimeofday(&stop, NULL); //end checking time
        printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
		printf("Microseconds taken: %d\n", stop.tv_usec - start.tv_usec);

        // PER ROW
        gettimeofday(&start, NULL); //start checking time
        printf("\nPer Row:\n");
        for(int i = 0; i< row1;i++){
            per_row(i, col2, col1);
        }
        gettimeofday(&stop, NULL); //end checking time
        printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
		printf("Microseconds taken: %d\n", stop.tv_usec - start.tv_usec);

        // PER ELEMENT
        gettimeofday(&start, NULL); //start checking time
        printf("\nPer Element:\n");
        for(int i = 0; i< row1 ;i++){
            for(int j = 0; j<col2; j++){
                element e;
                e.row = i;
                e.col = j;
                per_element(&e, col1);
            }
        }
		gettimeofday(&stop, NULL); //end checking time
        printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
		printf("Microseconds taken: %d\n", stop.tv_usec - start.tv_usec);
		
        printf("\n");
        for(int i = 0; i<row1; i++){
            for(int j = 0; j<col2;j++){
                printf("%d\t", c[i][j]);
            }
            printf("\n");
        }
	}else{
		printf("Dimensions are not compatible\n");
	}
	return 0;
}
