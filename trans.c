/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 2 - Optimizing Matrix Transpose
 *
 * Name(s):  
 * NetID(s): 
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 * and compute B = A^T.
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */

#include <stdio.h>
#include "support/cachelab.h"

#define BLOCK_SIZE32 8
#define BLOCK_SIZE64 8
int is_transpose(int M, int N, int A[M][N], int B[N][M]);


/*
 * transpose_submit - This is the transpose function that you will be graded
 *     on. Do not change the description string "Transpose submission", as the
 *     driver searches for that string to identify the transpose function to be
 *     graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[M][N], int B[N][M]) {
    if(M == 32){
	for (int i = 0; i < N; i += BLOCK_SIZE32){
            for (int j = 0; j < M; j += BLOCK_SIZE32){
                for (int x = i; x < i + BLOCK_SIZE32 && x < M; x++){
                    int row[BLOCK_SIZE32];
                    for (int y = j; y < j + BLOCK_SIZE32 && y < N; y++){
                        row[y-j] = A[x][y];
                    }
                    for (int num = 0; num < BLOCK_SIZE32; num++){
                        B[j+(x-i)][i+num]/*[x]*/ = row[num];
                    }
                }

                for(int a = 0; a < BLOCK_SIZE32; a++){
                    for(int b = a; b < BLOCK_SIZE32; b++){
                        int temp = B[j+a][i+b];
                        B[j+a][i+b] = B[j+b][i+a];
                        B[j+b][i+a] = temp;
                    }
                }
            }
        }
    }
    else{
        for (int i = 0; i < N; i += BLOCK_SIZE64){
            for (int j = 0; j < M; j += BLOCK_SIZE64){
                for (int x = i; x < i + BLOCK_SIZE64 && x < M; x++){
                    int row[BLOCK_SIZE64];
                    for (int y = j; y < j + BLOCK_SIZE64 && y < N; y++){
                        row[y-j] = A[x][y];
                    }
                    for (int num = 0; num < BLOCK_SIZE64; num++){
                        B[j+(x-i)][i+num]/*[x]*/ = row[num];
                    }
                }

                for(int a = 0; a < BLOCK_SIZE64; a++){
                    for(int b = a; b < BLOCK_SIZE64; b++){
                        int temp = B[j+a][i+b];
                        B[j+a][i+b] = B[j+b][i+a];
                        B[j+b][i+a] = temp;
                    }
                }
            }
        }
    }
}

// You can define additional transpose functions below. We've defined a simple
// one below to help you get started.

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M]) {
    int i, j, tmp;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

char trans_desc2[] = "Simple row-wise scan transpose";
/*void helpTranspose(int x, int y, int B[N][M]){ //Takes the newly filled B block (denoted by x and y boundaries) and transposes it within the B matrix
    for(int i = x; i < x + BLOCK_SIZE; i++){
        for(int j = y; j < y + BLOCK_SIZE; j++){
            int temp = B[i][j]; //Store the value of the matrix in a temporary varible
            B[i][j] = B[j][i];
            B[j][i] = temp;
        }
    }
}
*/
/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
}


/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M]) {
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
