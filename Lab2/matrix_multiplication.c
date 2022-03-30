//
//  main.c
//  matrix_multiplication
//
//  Created by AbdelRahman AbdelFattah on 29/03/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>

#define MAX_SIZE 20

struct element{
    int row;
    int col;
};


int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];
int c_matrix[MAX_SIZE][MAX_SIZE];
int c_row[MAX_SIZE][MAX_SIZE];
int c_element[MAX_SIZE][MAX_SIZE];
int row1 = -1,col1 = -1;
int row2 = -1,col2 = -1;

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
            free(temp);
        }
    }
    fclose(fp);
}

void write_files(char * filename){
    int row = row1;
    int col = col2;

    // PER MATRIX
    char * file_matrix = filename;
    strcat(file_matrix, "_per_matrix.txt");
    FILE *fp = fopen(file_matrix, "w");
    fprintf(fp, "Method: A thread per matrix\nrow=%d col=%d\n", row, col);
    for(int i = 0; i<row1; i++){
        for(int j = 0; j<col2;j++){
            fprintf(fp,"%d\t", c_matrix[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);

    // PER ROW
    char * file_row = filename;
    strcat(file_row, "_per_row.txt");
    fp = fopen(file_row, "w");
    fprintf(fp, "Method: A thread per row\nrow=%d col=%d\n", row, col);
    for(int i = 0; i<row1; i++){
        for(int j = 0; j<col2;j++){
            fprintf(fp,"%d\t", c_row[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);

    // PER ELEMENT
    char * file_element = filename;
    strcat(file_element, "_per_element.txt");
    fp = fopen(file_element, "w");
    fprintf(fp, "Method: A thread per element\nrow=%d col=%d\n", row, col);
    for(int i = 0; i<row1; i++){
        for(int j = 0; j<col2;j++){
            fprintf(fp,"%d\t", c_element[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

void per_matrix(){
    for(int i = 0; i<row1;i++){
        for(int j = 0; j<col2; j++){
            c_matrix[i][j] = 0;
            for(int k = 0; k<col1; k++){
                c_matrix[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
}

void * per_row(void* row){
	int current_row = (int) row;
    for(int j = 0; j<col2; j++){
        c_row[current_row][j] = 0;
        for(int k = 0; k<col1; k++){
            c_row[current_row][j]+=a[current_row][k]*b[k][j];
        }
    }
	pthread_exit(NULL);
}

void * per_element(void* element){
    struct element * current_element;
    current_element = (struct element *) element;
    int row = current_element -> row;
    int col = current_element -> col;
    free(element);
    c_element[row][col] = 0;
    for(int k = 0; k<col1; k++){
        c_element[row][col]+=a[row][k]*b[k][col];
    }
    pthread_exit(NULL);
}


int main(int argc, const char * argv[]) {
	struct timeval stop, start;
	char * mat1 = malloc(64);
	char * mat2 = malloc(64);
	char * matout = malloc(64);
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
    read_file(mat1, &row1, &col1, a);
    read_file(mat2, &row2, &col2, b);
    free(mat1);
    free(mat2);
	if(check_if_compatible(col1,row2)){
        // PER MATRIX
        int threads = 0;
		gettimeofday(&start, NULL); //start checking time
        printf("\nPer Matrix:\n");
        threads++;
		per_matrix();
        gettimeofday(&stop, NULL); //end checking time
        printf("Number of threads: %d\n", threads);
		printf("Microseconds taken: %ld\n", stop.tv_usec - start.tv_usec);

        // PER ROW
        threads = 0;
        pthread_t threads_per_row[MAX_SIZE];
        gettimeofday(&start, NULL); //start checking time
        printf("\nPer Row:\n");
        for(int i = 0; i< row1;i++){
            threads++;
            int rc = pthread_create(&threads_per_row[i], NULL, per_row, (void *) i);
            if(rc){
                printf("ERROR; from pthread_create()\n");
            }
        }
        gettimeofday(&stop, NULL); //end checking time
        printf("Number of threads: %d\n", threads);
		printf("Microseconds taken: %ld\n", stop.tv_usec - start.tv_usec);
        for(int i =0 ;i<MAX_SIZE; i++){
            pthread_join(threads_per_row[i],NULL);
        }
        
        // PER ELEMENT
        threads = 0;
        pthread_t threads_per_element[MAX_SIZE * MAX_SIZE];
        gettimeofday(&start, NULL); //start checking time
        printf("\nPer Element:\n");
        for(int i = 0; i< row1 ;i++){
            for(int j = 0; j<col2; j++){
                struct element * e = malloc(sizeof(struct element));
                e->row = i;
                e->col = j;
                threads++;
                int rc = pthread_create(&threads_per_element[i*col2 + j], NULL, per_element, (void *) e);
                if(rc){
                    printf("ERROR; from pthread_create()\n");
                }
            }
        }
		gettimeofday(&stop, NULL); //end checking time
        printf("Number of threads: %d\n", threads);
		printf("Microseconds taken: %ld\n", stop.tv_usec - start.tv_usec);
        for(int i =0 ;i<MAX_SIZE * MAX_SIZE; i++){
            pthread_join(threads_per_element[i],NULL);
        }
        write_files(matout);
        free(matout);
	}else{
		printf("Dimensions are not compatible\n");
	}
	return 0;
}
