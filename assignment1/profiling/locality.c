#include <stdio.h>
#include <time.h>
#include <stdlib.h>
void row_sums( double * sums, const double ** matrix, size_t nrs, size_t ncs){
    for ( size_t ix=0; ix < nrs; ++ix ){
        double sum = 0;
        for ( size_t jx=0; jx < ncs; ++jx ){
            sum += matrix[ix][jx];
        }
        sums[ix] = sum;
    }   
}

void col_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs){
    for ( size_t jx=0; jx < ncs; ++jx ){
        double sum = 0;
        for ( size_t ix=0; ix < nrs; ++ix ){
            sum += matrix[ix][jx];
        }
        sums[jx] = sum;
    }
}


void col_sums_fast( double * sums, const double ** matrix, size_t nrs, size_t ncs){
    for ( size_t ix=0; ix < nrs; ++ix ){
        for ( size_t jx=0; jx < ncs; ++jx ){
            sums[ix] += matrix[ix][jx];
        }
    }   
}



double ** mk_mat(size_t col_size, size_t row_size){
    double **matrix = malloc(sizeof(double *) * row_size);
    matrix[0] = malloc(row_size * col_size * sizeof(double));
    for(size_t i = 0; i < col_size; i++)
       matrix[i] = matrix[0] + i*col_size; 

    for(size_t ix = 0; ix < col_size; ++ix){
        for(size_t jx = 0; jx < row_size; ++jx){
            *((*(matrix)) + (ix * col_size + jx)) = 42.0/1000;
        }
    }
    return matrix;
}


int main(){
    size_t matrix_size = 1000;
    size_t nbr = 100;
    double *row_sum;
    double *col_sum;
    double *col_sum_fast;
    const double **mat;
    time_t start;
    time_t end;
    double diff;
    double averageTime;

    row_sum = (double*)malloc(matrix_size * sizeof(double));
    col_sum = (double*)malloc(matrix_size * sizeof(double));
    col_sum_fast = (double*)malloc(matrix_size * sizeof(double));
    mat =(const double **) mk_mat(matrix_size, matrix_size);
    
    //---------------------- Naive row sum ----------------------
    for(size_t i = 0; i < nbr; i++)
        row_sums(row_sum, mat, matrix_size, matrix_size);
    
    start = clock();
    for(size_t i = 0; i < nbr; i++)
        row_sums(row_sum, mat, matrix_size, matrix_size);
 
    end = clock();
    diff = ((end - start) / (double) CLOCKS_PER_SEC) * 1000; //ms

    averageTime = diff/(double)nbr;
    printf("Naive row sum average time in ms: %f\n", averageTime); 
    
    //---------------------- Naive col sum ----------------------
    
    for(size_t i = 0; i < nbr; i++)
        col_sums(col_sum, mat, matrix_size, matrix_size);
    
    start = clock();
    for(size_t i = 0; i < nbr; i++)
        col_sums(col_sum, mat, matrix_size, matrix_size);
 
    end = clock();
    diff = ((end - start) / (double) CLOCKS_PER_SEC) * 1000; //ms

    averageTime = diff/(double)nbr;
    printf("Naive column sum average time in ms: %f\n", averageTime); 
    
    
    //---------------------- right col sum ----------------------
    
    for(size_t i = 0; i < nbr; i++)
        col_sums_fast(col_sum_fast, mat, matrix_size, matrix_size);
    
    start = clock();
    for(size_t i = 0; i < nbr; i++)
        col_sums_fast(col_sum_fast, mat, matrix_size, matrix_size);
 
    end = clock();
    diff = ((end - start) / (double) CLOCKS_PER_SEC) * 1000; //ms

    averageTime = diff/(double)nbr;
    printf("fast column sum average time in ms: %f\n", averageTime); 
    
    
    free(mat);
    free(row_sum);
    free(col_sum);
    free(col_sum_fast);
    return 0;   
}


