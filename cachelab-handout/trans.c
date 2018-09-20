/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp, index;
    int row_block, col_block;
    int b1, b2, b3, b4;
    if (M == 32) {
        for (row_block = 0; row_block < N; row_block += 8) {
            for (col_block = 0; col_block < M; col_block += 8) {
                for (i = row_block; i < row_block + 8; ++i) {
                    for (j = col_block; j < col_block + 8; ++j) {
                        if (i != j) {
                            B[j][i] = A[i][j];
                        } else {
                            tmp = A[i][j];
                            index = i;
                        }
                    }
                    if (row_block == col_block) {
                        B[index][index] = tmp;
                    }
                }
            }
        }
    } else if (M == 64) {
        for (row_block = 0; row_block < N; row_block += 8) {
            for (col_block = 0; col_block < M; col_block += 8) {
                for (i = row_block; i < row_block + 4; ++i) {
                    for (j = col_block; j < col_block + 4; ++j) {
                        if (i != j) {
                            B[j][i] = A[i][j];
                        } else {
                            tmp = A[i][j];
                            index = i;
                        }
                    }
                    if (row_block == col_block) {
                        B[index][index] = tmp;
                    }
                }

                for (i = row_block; i < row_block + 4; ++i) {
                     b1 = A[i][col_block + 4];
                     b2 = A[i][col_block + 5];
                     b3 = A[i][col_block + 6];
                     b4 = A[i][col_block + 7];
                     B[col_block + 4][i] = b1;
                     B[col_block + 5][i] = b2;
                     B[col_block + 6][i] = b3;
                     B[col_block + 7][i] = b4;
                }

                for (i = row_block + 4; i < row_block + 8; ++i) {
                     b1 = A[i][col_block];
                     b2 = A[i][col_block + 1];
                     b3 = A[i][col_block + 2];
                     b4 = A[i][col_block + 3];
                     B[col_block][i] = b1;
                     B[col_block + 1][i] = b2;
                     B[col_block + 2][i] = b3;
                     B[col_block + 3][i] = b4;
                }

                /*for (i = row_block; i < row_block + 4; ++i) {
                    for (j = col_block + 4; j < col_block + 8; ++j) {
                        if (i != j - 4) {
                            B[j][i] = A[i][j];
                        } else {
                            if (i == row_block) {
                                b1 = A[i][j];
                            } else if (i == row_block + 1) {
                                b2 = A[i][j];
                            } else if (i == row_block + 2) {
                                b3 = A[i][j];
                            } else if (i == row_block + 3) {
                                b4 = A[i][j];
                            }
                        }
                    }
                    if (i == row_block) {
                        B[col_block + 4][i] = b1;
                    }
                    if (i == row_block + 1) {
                        B[col_block + 5][i] = b2;
                    }
                    if (i == row_block + 2) {
                        B[col_block + 6][i] = b3;
                    }
                    if (i == row_block + 3) {
                        B[col_block + 7][i] = b4;
                    }
                }

                for (i = row_block + 4; i < row_block + 8; ++i) {
                    for (j = col_block; j < col_block + 4; ++j) {
                        if (i - 4 != j) {
                            B[j][i] = A[i][j];
                        } else {
                            if (i == row_block + 4) {
                                b1 = A[i][j];
                            } else if (i == row_block + 5) {
                                b2 = A[i][j];
                            } else if (i == row_block + 6) {
                                b3 = A[i][j];
                            } else if (i == row_block + 7) {
                                b4 = A[i][j];
                            }
                        }
                    }
                    if (i == row_block + 4) {
                        B[col_block][i] = b1;
                    }
                    if (i == row_block + 5) {
                        B[col_block + 1][i] = b2;
                    }
                    if (i == row_block + 6) {
                        B[col_block + 2][i] = b3;
                    }
                    if (i == row_block + 7) {
                        B[col_block + 3][i] = b4;
                    }
                }*/

                for (i = row_block + 4; i < row_block + 8; ++i) {
                    for (j = col_block + 4; j < col_block + 8; ++j) {
                        if (i != j) {
                            B[j][i] = A[i][j];
                        } else {
                            tmp = A[i][j];
                            index = i;
                        }
                    }
                    if (row_block == col_block) {
                        B[index][index] = tmp;
                    }
                }
            }
        }
    } else {
        for (row_block = 0; row_block < N; row_block += 16) {
            for (col_block = 0; col_block < M; col_block += 16) {
                for (i = row_block; i < row_block + 16 && i < N; ++i) {
                    for (j = col_block; j < col_block + 16 && j < M; ++j) {
                        if (i != j) {
                            B[j][i] = A[i][j];
                        } else {
                            tmp = A[i][j];
                            index = i;
                        }
                    }
                    if (row_block == col_block) {
                        B[index][index] = tmp;
                    }
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
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
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

