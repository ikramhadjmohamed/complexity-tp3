#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generer.h"
#include "sousMat.h"

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

        /* 3. clock() autour de sousMat1(...) -> temps1 */
        int pi, pj;
        clock_t d1 = clock();
        sousMat1(A, n, n, B, np, mp, &pi, &pj);
        clock_t f1t = clock();
        double temps1 = (double)(f1t - d1) / CLOCKS_PER_SEC;

        /* 4. clock() autour de sousMat2(...) -> temps2 */
        clock_t d2 = clock();
        sousMat2(A, n, n, B, np, mp, &pi, &pj);
        clock_t f2t = clock();
        double temps2 = (double)(f2t - d2) / CLOCKS_PER_SEC;

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

        int pi, pj;
        clock_t d1 = clock();
        sousMat1(A2mat, n, m, B, npv, mpv, &pi, &pj);
        clock_t f1t = clock();
        double temps1 = (double)(f1t - d1) / CLOCKS_PER_SEC;

        clock_t d2 = clock();
        sousMat2(A2mat, n, m, B, npv, mpv, &pi, &pj);
        clock_t f2t = clock();
        double temps2 = (double)(f2t - d2) / CLOCKS_PER_SEC;

        fprintf(f2, "%d;%f;%f\n", npv, temps1, temps2);
        printf("[Exp2] n'=%d  sousMat1=%f  sousMat2=%f\n", npv, temps1, temps2);

        liberer(B, npv);
    }

    liberer(A2mat, n);
    fclose(f2);

    return 0;
}