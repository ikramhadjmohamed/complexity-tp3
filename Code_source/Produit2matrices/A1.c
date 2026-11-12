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
    int n, m, p;

    srand((unsigned)time(NULL));   /* initialise le generateur aleatoire */
    
    printf("n, m, p : ");
    scanf("%d %d %d", &n, &m, &p);

    int **A = allouer(n, m);
    int **B = allouer(m, p);
    int **C = allouer(n, p);

    remplir(A, n, m);
    remplir(B, m, p);
    produit(A, B, C, n, m, p);

    if (n <= 5 && m <= 5 && p <= 5) {   /* affichage seulement si petit */
        printf("A:\n"); afficher(A, n, m);
        printf("B:\n"); afficher(B, m, p);
        printf("C:\n"); afficher(C, n, p);
    }

    liberer(A, n); liberer(B, m); liberer(C, n);
    return 0;
}