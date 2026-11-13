/* Premier indice idx tel que T[idx] >= val (m si aucun) */
int borneInf(int *T, int m, int val) {
    int lo = 0, hi = m;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (T[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

int sousMat2(int **A, int n, int m, int **B, int np, int mp, int *pi, int *pj) {
    for (int i = 0; i <= n - np; i++) {
        int j = borneInf(A[i], m, B[0][0]);

        while (j <= m - mp && A[i][j] == B[0][0]) {
            int egal = 1;
            for (int x = 0; x < np && egal; x++) {
                /* rejet rapide : premier et dernier element de la ligne */
                if (A[i + x][j] != B[x][0] || A[i + x][j + mp - 1] != B[x][mp - 1])
                    egal = 0;
                else
                    for (int y = 1; y < mp - 1 && egal; y++)
                        if (A[i + x][j + y] != B[x][y])
                            egal = 0;
            }
            if (egal) { *pi = i; *pj = j; return 1; }
            j++;
        }
    }
    return 0;
}