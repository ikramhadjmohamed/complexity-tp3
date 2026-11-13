#include <stdio.h>
/* Cherche B (np x mp) dans A (n x m).
   Retourne 1 si trouvee (position dans *pi, *pj), 0 sinon. */
int sousMat1(int **A, int n, int m, int **B, int np, int mp, int *pi, int *pj) {
    for (int i = 0; i <= n - np; i++) {            /* positions de depart valides */
        for (int j = 0; j <= m - mp; j++) {

            int egal = 1;                          /* on suppose que ca correspond */
            for (int x = 0; x < np && egal; x++)
                for (int y = 0; y < mp && egal; y++)
                    if (A[i + x][j + y] != B[x][y])
                        egal = 0;                  /* un element differe : on abandonne */

            if (egal) {
                *pi = i;
                *pj = j;
                return 1;
            }
        }
    }
    return 0;
}
