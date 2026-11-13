#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generer.h"
#include "sousMat.h"

typedef int (*FctSousMat)(int **, int, int, int **, int, int, int *, int *);

double mesurer_sousMat(FctSousMat f, int **A, int n, int m, int **B, int np, int mp) {
    int pi, pj;
    volatile int sink = 0;
    long reps = 1;
    for (;;) {
        clock_t d = clock();
        for (long r = 0; r < reps; r++) {
            f(A, n, m, B, np, mp, &pi, &pj);
            sink += pi;
        }
        double t = (double)(clock() - d) / CLOCKS_PER_SEC;
        if (t >= 0.05 || reps >= 1000000000L) return t / reps;
        reps *= 10;
    }
}

int main(void) {
    srand(42);
    int tailles[] = {100, 150, 200, 300, 400, 500, 600, 700, 800,
                      900, 1000, 1200, 1400, 1600, 1800, 2000};
    int nb = sizeof(tailles) / sizeof(tailles[0]);

    /* ===================================================== */
    /* Experience 1 : n, m varient, n', m' fixes             */
    /* ===================================================== */

    FILE *f1 = fopen("resultats_exp1.csv", "w");
    if (f1 == NULL) {
        printf("Erreur ouverture resultats_exp1.csv\n");
        return 1;
    }
    fprintf(f1, "n;temps_sousMat1;temps_sousMat2\n");

    int np = 10, mp = 10;   /* (n', m') fixe */

    for (int t = 0; t < nb; t++) {
        int n = tailles[t];

        /* 1. allouer A (n x n), remplirTrie(A, n, n) */
        int **A = allouer(n, n);
        remplirTrie(A, n, n);

        /* 2. extraireBloc(A, i0, j0, np, mp) -> B, avec i0 <= n-np, j0 <= n-mp */
        int i0 = (n - np) / 2;
        int j0 = (n - mp) / 2;
        int **B = extraireBloc(A, i0, j0, np, mp);

        double temps1 = mesurer_sousMat(sousMat1, A, n, n, B, np, mp);
        double temps2 = mesurer_sousMat(sousMat2, A, n, n, B, np, mp);

        fprintf(f1, "%d;%f;%f\n", n, temps1, temps2);
        printf("[Exp1] n=%d  sousMat1=%f  sousMat2=%f\n", n, temps1, temps2);

        liberer(A, n);
        liberer(B, np);
    }
    fclose(f1);

    /* ===================================================== */
    /* Experience 2 : n', m' varient, n, m fixes              */
    /* ===================================================== */

    FILE *f2 = fopen("resultats_exp2.csv", "w");
    if (f2 == NULL) {
        printf("Erreur ouverture resultats_exp2.csv\n");
        return 1;
    }
    fprintf(f2, "np;temps_sousMat1;temps_sousMat2\n");

    int n = 2500, m = 2500;   /* (n, m) fixe */

    /* A generee une seule fois, en dehors de la boucle */
    int **A2mat = allouer(n, m);
    remplirTrie(A2mat, n, m);

    for (int t = 0; t < nb; t++) {
        int npv = tailles[t];
        int mpv = tailles[t];

        int i0 = (n - npv) / 2;
        int j0 = (m - mpv) / 2;
        int **B = extraireBloc(A2mat, i0, j0, npv, mpv);

        double temps1 = mesurer_sousMat(sousMat1, A2mat, n, m, B, npv, mpv);
        double temps2 = mesurer_sousMat(sousMat2, A2mat, n, m, B, npv, mpv);


        fprintf(f2, "%d;%f;%f\n", npv, temps1, temps2);
        printf("[Exp2] n'=%d  sousMat1=%f  sousMat2=%f\n", npv, temps1, temps2);

        liberer(B, npv);
    }

    liberer(A2mat, n);
    fclose(f2);

    return 0;
}