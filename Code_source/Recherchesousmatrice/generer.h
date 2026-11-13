#ifndef GENERER_H
#define GENERER_H

int **allouer(int l, int c);
void liberer(int **M, int l);
void remplirTrie(int **M, int l, int c);
int **extraireBloc(int **A, int i0, int j0, int np, int mp);

#endif