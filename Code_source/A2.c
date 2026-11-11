#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Alloue une matrice l x c (tableau de pointeurs vers des lignes) */
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

void remplir(int **M, int l, int c) {
    for (int i = 0; i < l; i++)
        for (int j = 0; j < c; j++)
            M[i][j] = rand() % 10;   /* valeurs 0..9 */
}

void afficher(int **M, int l, int c) {
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) printf("%4d ", M[i][j]);
        printf("\n");
    }
}

/* C(n,p) = A(n,m) x B(m,p) */
void produit(int **A, int **B, int **C, int n, int m, int p) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++) {
            int s = 0;
            for (int k = 0; k < m; k++)
                s += A[i][k] * B[k][j];
            C[i][j] = s;
        }
}

int main(void) {
    srand(42);
    int tailles[] = {100, 200, 400, 800, 1000};
    int nb = sizeof(tailles) / sizeof(tailles[0]);

    for (int t = 0; t < nb; t++) {
        int n = tailles[t];

        int **A = allouer(n, n);
        int **B = allouer(n, n);
        int **C = allouer(n, n);

        remplir(A, n, n);
        remplir(B, n, n);

        clock_t debut = clock();
        produit(A, B, C, n, n, n);
        clock_t fin = clock();

        double temps = (double)(fin - debut) / CLOCKS_PER_SEC;
        printf("%d\t%f\n", n, temps);

        liberer(A, n); liberer(B, n); liberer(C, n);
    }
    return 0;
}