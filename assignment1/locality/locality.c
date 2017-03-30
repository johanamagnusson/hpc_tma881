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

double ** mk_mat(size_t col_size, size_t row_size){
    double **matrix = malloc(sizeof(double) * col_size * row_size);
    for(size_t ix = 0; ix < col_size; ++ix){
                
        for(size_t jx = 0; jx < row_size; ++jx){
            
        }
    }
}


int main(){
    size_t matrix_size = 100;
    double **matrix = mk_mat(matrix_size, matrix_size);
    free(matrix);
    return 0;   
}


