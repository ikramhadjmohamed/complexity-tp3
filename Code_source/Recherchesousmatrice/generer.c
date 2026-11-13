#include <stdlib.h>
#include "generer.h"

int **allouer(int l, int c) {
    int **M = (int **)malloc(l * sizeof(int *));
    for (int i = 0; i < l; i++)
        M[i] = (int *)calloc(c, sizeof(int));
    return M;
}

void liberer(int **M, int l) {
    for (int i = 0; i < l; i++) free(M[i]);
    free(M);
}

int comparer(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void remplirTrie(int **M, int l, int c) {
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++)
            M[i][j] = rand() % 100;
        qsort(M[i], c, sizeof(int), comparer);
    }
}

int **extraireBloc(int **A, int i0, int j0, int np, int mp) {
    int **B = allouer(np, mp);
    for (int x = 0; x < np; x++)
        for (int y = 0; y < mp; y++)
            B[x][y] = A[i0 + x][j0 + y];
    return B;
}