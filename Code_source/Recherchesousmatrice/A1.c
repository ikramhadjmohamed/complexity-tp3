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

int main(void) {
    int a[3][4] = {{1,2,3,4},{5,6,7,8},{9,1,2,3}};
    int b[2][2] = {{2,3},{6,7}};

    int *A[3], *B[2];
    for (int i = 0; i < 3; i++) A[i] = a[i];
    for (int i = 0; i < 2; i++) B[i] = b[i];

    int pi, pj;
    if (sousMat1(A, 3, 4, B, 2, 2, &pi, &pj))
        printf("Trouvee en (%d, %d)\n", pi, pj);
    else
        printf("Non trouvee\n");
    return 0;
}