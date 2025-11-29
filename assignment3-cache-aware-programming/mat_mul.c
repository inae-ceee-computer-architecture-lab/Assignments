/*******************************************************************
 * File: mat_mul.c
 * Description: Cache aware matrix multiplication
 *******************************************************************/

#include <stdio.h>
#include <chrono>
#include <stdlib.h>
#include "helper.h"

#define TILE_SIZE 100


/***************************************
 * Naive O(N^3)
 ***************************************/
void naive_mat_mul(double *A, double *B, double *C, int size) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                C[i * size + j] += A[i * size + k] * B[k * size + j];
            }
        }
    }
}


/***************************************
 * Task 1A – Loop-optimized i-k-j order
 ***************************************/
void loop_opt_mat_mul(double *A, double *B, double *C, int size) {

    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            double a_val = A[i * size + k];
            for (int j = 0; j < size; j++) {
                C[i * size + j] += a_val * B[k * size + j];
            }
        }
    }
}


/***************************************
 * Task 1B – Tiled / Blocked O(N^3)
 ***************************************/
void tile_mat_mul(double *A, double *B, double *C, int size, int block_size) {

    for (int ii = 0; ii < size; ii += block_size) {
        for (int kk = 0; kk < size; kk += block_size) {
            for (int jj = 0; jj < size; jj += block_size) {

                int i_end = (ii + block_size < size) ? ii + block_size : size;
                int k_end = (kk + block_size < size) ? kk + block_size : size;
                int j_end = (jj + block_size < size) ? jj + block_size : size;

                for (int i = ii; i < i_end; i++) {
                    for (int k = kk; k < k_end; k++) {
                        double a_val = A[i * size + k];
                        for (int j = jj; j < j_end; j++) {
                            C[i * size + j] += a_val * B[k * size + j];
                        }
                    }
                }
            }
        }
    }
}

/***************************************
 * Task 1C – Combined (tiling + loop-opt)
 ***************************************/
void combination_mat_mul(double *A, double *B, double *C, int size, int block_size) {

    for (int ii = 0; ii < size; ii += block_size) {
        for (int kk = 0; kk < size; kk += block_size) {
            for (int jj = 0; jj < size; jj += block_size) {

                int i_end = (ii + block_size < size) ? ii + block_size : size;
                int k_end = (kk + block_size < size) ? kk + block_size : size;
                int j_end = (jj + block_size < size) ? jj + block_size : size;

                for (int i = ii; i < i_end; i++) {
                    for (int k = kk; k < k_end; k++) {
                        double a_val = A[i * size + k];
                        for (int j = jj; j < j_end; j++) {
                            C[i * size + j] += a_val * B[k * size + j];
                        }
                    }
                }
            }
        }
    }
}


/*************************************************************
 * Main (Timing and speedup)
 *************************************************************/
int main(int argc, char **argv) {

    if (argc <= 1) {
        printf("Usage: %s <matrix_dimension>\n", argv[0]);
        return 0;
    }

    int size = atoi(argv[1]);

    long long time_naive, time_loop, time_tiled, time_combined;

    double *A = (double *)malloc(size * size * sizeof(double));
    double *B = (double *)malloc(size * size * sizeof(double));
    double *C = (double *)calloc(size * size, sizeof(double));

    srand(time(NULL));

    initialize_matrix(A, size, size);
    initialize_matrix(B, size, size);


    /*******************
     * Naive
     *******************/
    initialize_result_matrix(C, size, size);

    auto start = std::chrono::high_resolution_clock::now();
    naive_mat_mul(A, B, C, size);
    auto end = std::chrono::high_resolution_clock::now();

    time_naive = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    printf("Naive matmul: %lld microseconds\n\n", time_naive);


#ifdef OPTIMIZE_LOOP_OPT
    /*******************
     * Loop-optimized
     *******************/
    initialize_result_matrix(C, size, size);

    start = std::chrono::high_resolution_clock::now();
    loop_opt_mat_mul(A, B, C, size);
    end = std::chrono::high_resolution_clock::now();

    time_loop = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    printf("Loop-optimized matmul: %lld microseconds\n", time_loop);

    if (time_loop > 0)
        printf("Speedup: %.4f\n\n", (double)time_naive / time_loop);
    else
        printf("Speedup: N/A (time_loop = 0)\n\n");
#endif


#ifdef OPTIMIZE_TILING
    /*******************
     * Tiled
     *******************/
    initialize_result_matrix(C, size, size);

    start = std::chrono::high_resolution_clock::now();
    tile_mat_mul(A, B, C, size, TILE_SIZE);
    end = std::chrono::high_resolution_clock::now();

    time_tiled = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    printf("Tiled matmul: %lld microseconds\n", time_tiled);

    if (time_tiled > 0)
        printf("Speedup: %.4f\n\n", (double)time_naive / time_tiled);
    else
        printf("Speedup: N/A (time_tiled = 0)\n\n");
#endif


#ifdef OPTIMIZE_COMBINED
    /*******************
     * Combined (tiling + loop-opt)
     *******************/
    initialize_result_matrix(C, size, size);

    start = std::chrono::high_resolution_clock::now();
    combination_mat_mul(A, B, C, size, TILE_SIZE);
    end = std::chrono::high_resolution_clock::now();

    time_combined = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    printf("Combined matmul: %lld microseconds\n", time_combined);

    if (time_combined > 0)
        printf("Speedup: %.4f\n\n", (double)time_naive / time_combined);
    else
        printf("Speedup: N/A (time_combined = 0)\n\n");
#endif


    free(A);
    free(B);
    free(C);

    return 0;
}

