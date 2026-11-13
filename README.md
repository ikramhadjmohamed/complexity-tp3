## Objectif

Étudier expérimentalement la complexité polynomiale de :

- **Partie A** : le produit de deux matrices carrées (n × n), en temps d'exécution et en
  espace mémoire.
- **Partie B** : deux algorithmes de recherche d'une sous-matrice B dans une matrice A
  (`sousMat1` naïf, `sousMat2` par lignes triées + dichotomie), en faisant varier
  séparément la taille de la grande matrice (n, m) puis celle de la sous-matrice cherchée
  (n', m').

Les mesures sont comparées à la complexité théorique.

## Structure du dépôt

```
TP3
  Code_source
    Produit2matrices
      A1.c                  produit de 2 matrices (allocation dynamique, verification des dimensions)
      A2.c                  mesure des temps d'execution du produit, export CSV
      graphe.py             courbe T(n) et regression log-log
    Recherchesousmatrice
      A1.c                  sousMat1 (recherche naive) + test unitaire
      A2.c                  sousMat2 (dichotomie, lignes triees) + test unitaire
      generer.h             declarations : allouer, liberer, remplirTrie, extraireBloc
      generer.c             generation des matrices (allocation, remplissage trie, extraction de bloc)
      sousMat.h             declarations : sousMat1, sousMat2
      sousMat.c             sousMat1 et sousMat2 (sans main, reutilisees par test.c)
      test.c                mesures des 2 experiences, export CSV
      graphe_sousmat.py     courbes sousMat1 vs sousMat2 et exposants experimentaux
  Resultats_experimentaux
    resultats_produit.csv   resultats de la partie A (produit de matrices)
    resultats_exp1.csv      resultats partie B, experience 1 (n, m variables)
    resultats_exp2.csv      resultats partie B, experience 2 (n', m' variables)
    graphe_produit.png      graphe T(n) de la partie A
    graphe_exp1.png         graphe de l'experience 1
    graphe_exp2.png         graphe de l'experience 2
  README.md
```

## Compilation et exécution

Toutes les commandes se lancent depuis le dossier concerné (Windows, CMD).

**Partie A — Produit de matrices**
```
cd Code_source\Produit2matrices
gcc A1.c -o A1.exe
A1.exe

gcc A2.c -o A2.exe
A2.exe

python graphe.py
```

**Partie B — Recherche de sous-matrice**
```
cd Code_source\Recherchesousmatrice
gcc A1.c -o A1.exe
A1.exe

gcc A2.c -o A2.exe
A2.exe

gcc test.c generer.c sousMat.c -o test.exe
test.exe

python graphe_sousmat.py
```
L'ordre est important pour la partie B : `test.c` dépend de `generer.c` et `sousMat.c`
(les deux doivent compiler sans erreur avant de lancer les mesures).

## Jeux de données

Aucun fichier de données externe : chaque programme génère ses matrices en mémoire,
avec une graine fixe (`srand(42)`) pour des résultats reproductibles.

| Partie | Génération | Tailles utilisées |
|---|---|---|
| A — Produit | `rand() % 100`, matrices carrées n × n | 100, 150, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1200, 1400, 1600, 1800, 2000 (16 valeurs) |
| B — Exp. 1 (n, m variables) | A trié par ligne (`qsort`), B extraite comme bloc de A | A : mêmes 16 valeurs que la partie A — B fixe à 10 × 10 |
| B — Exp. 2 (n', m' variables) | idem | A fixe à 2500 × 2500 — B : mêmes 16 valeurs (100 à 2000) |

B est toujours extraite directement comme un bloc de A : elle y est donc garantie présente,
et ses lignes sont déjà triées (puisqu'issues de lignes triées de A).

## Méthode de mesure

- Seul l'appel de la fonction mesurée est chronométré : l'allocation, le remplissage et la
  libération des matrices sont exclus.
- Le temps est mesuré avec `clock()` (`time.h`). Une seule exécution étant souvent trop
  courte, la fonction est répétée plusieurs fois et le temps total est divisé par le
  nombre de répétitions. Le nombre de répétitions augmente automatiquement (par palier de
  10) jusqu'à ce que la mesure dure au moins 50 ms.
- `sousMat1` et `sousMat2` ne modifient pas leurs arguments (lecture seule de A et B),
  donc aucune copie n'est nécessaire entre les répétitions.

## Complexité théorique

### Partie A : produit de deux matrices

Trois boucles imbriquées (i, j, k) :

    T(n, m, p) = O(n · m · p)

Pour des matrices carrées (n = m = p) :

    T(n) = O(n³)

Espace mémoire (trois matrices n × m, m × p, n × p) :

    Espace = O(nm + mp + np) = O(n²) pour n = m = p

### Partie B : recherche d'une sous-matrice

**sousMat1 (naïve, A et B non triées).** On essaie chaque position de départ (i, j) où le
bloc de B peut commencer dans A, et pour chaque position on compare les n'·m' éléments de
B avec le bloc correspondant de A.

1. Nombre de positions de départ possibles : (n - n' + 1) × (m - m' + 1).
2. Coût de la comparaison à une position, au pire (aucune différence trouvée avant la
   dernière case) : n' × m' comparaisons.

        comparaisons (pire cas) = (n - n' + 1) × (m - m' + 1) × n' × m'

Donc, au pire cas :

    T_sousMat1(n, m, n', m') = O( (n - n' + 1)(m - m' + 1) · n'·m' )

**sousMat2 (A et B triées par ligne).** Pour chaque ligne i de A, une recherche
dichotomique localise en O(log m) la ou les positions j où `A[i][j] = B[0][0]` (premier
élément de la ligne 0 de B). Chaque candidat retenu est ensuite vérifié, avec un rejet
rapide dès que le premier ou le dernier élément d'une ligne diffère :

1. Dichotomie par ligne de A : O(log m) par ligne, soit O((n - n' + 1) · log m) au total
   pour localiser les candidats.
2. Vérification d'un candidat : O(1) si rejeté immédiatement (premier ou dernier élément
   différent), O(n'·m') au pire si le candidat est presque identique à B ou s'il s'agit du
   bon candidat.

Donc, au pire cas (tous les candidats passent le rejet rapide, cas défavorable avec
beaucoup de doublons) :

    T_sousMat2(n, m, n', m') = O( (n - n' + 1) · log(m) + k · n'·m' )

où k est le nombre de candidats qui passent le rejet rapide (k = 1 en moyenne sur des
données sans doublon).

**Comparaison des deux facteurs.** Dans les deux formules, deux quantités évoluent en sens
opposé selon ce qu'on fait varier :

| Paramètre qui augmente | Effet sur le nombre de positions | Effet sur le coût par position |
|---|---|---|
| n, m (A grandit, n', m' fixes) | augmente | inchangé |
| n', m' (B grandit, n, m fixes) | diminue | augmente |

C'est cette tension entre les deux effets qui explique les comportements observés
expérimentalement (voir Observations), en particulier le comportement non monotone de
`sousMat1` dans l'expérience 2.

**Exemple de valeurs (ordre de grandeur du nombre de positions testées).**

| n = m | n' = m' | Positions possibles (n-n'+1)² |
|---|---|---|
| 2000 | 10 | 1 980 025 |
| 2500 | 100 | 5 764 801 |
| 2500 | 2000 | 251 001 |

Le nombre de positions chute fortement quand n' se rapproche de n (dernière ligne), ce qui
réduit le travail de `sousMat1` sur ce facteur, même si chaque vérification devient alors
plus coûteuse.


## Vérification expérimentale

**Produit de matrices.** Une régression log-log (`T = c·n^a`) sur les mesures donne un
exposant expérimental a ≈ 3,8 (ensemble des points), et jusqu'à 4,2 en ne gardant que
n ≥ 400 — contre a = 3 en théorie. L'écart s'explique par les effets de hiérarchie
mémoire (cache) : tant que les trois matrices tiennent dans le cache du processeur, le
coût par opération est quasi constant ; au-delà d'une certaine taille, les accès à des
zones mémoire plus éloignées deviennent plus coûteux, ce qui accélère la croissance du
temps réel au-delà de ce que prévoit le simple compte d'opérations arithmétiques.

**Recherche de sous-matrice.** `sousMat2` est systématiquement plus rapide que `sousMat1`
dans les deux expériences, confirmant l'intérêt du tri et de la dichotomie.

## Résultats

### Partie A : `Resultats_experimentaux\resultats_produit.csv`

Colonnes : `n;temps` (16 lignes). Le temps est celui du produit `C = A × B` pour des
matrices n × n, en secondes.

### Partie B

- `resultats_exp1.csv` — colonnes `n;temps_sousMat1;temps_sousMat2` (16 lignes,
  n = m variables, n' = m' = 10 fixe).
- `resultats_exp2.csv` — colonnes `np;temps_sousMat1;temps_sousMat2` (16 lignes,
  n' = m' variables, n = m = 2500 fixe).

## Observations

**Partie A — Produit de matrices**

Le temps croît avec n de façon nettement plus rapide que linéaire ou quadratique, cohérent
avec une complexité cubique. Entre n = 1000 et n = 2000, le temps est multiplié par un
facteur proche de ce que prévoit (2000/1000)³ = 8, avec cependant un écart croissant aux
grandes valeurs de n, dû aux effets de cache mentionnés plus haut.

**Partie B — Expérience 1 (A variable, B fixe à 10×10)**

`sousMat2` reste quasiment insensible à la croissance de n (grâce à la dichotomie), alors
que `sousMat1` voit son temps croître nettement avec n (plus de positions de départ à
essayer). L'écart entre les deux fonctions grandit avec n : environ ×10 à n = 100, environ
×20 à n = 2000. C'est le scénario le plus favorable à `sousMat2`.

**Partie B — Expérience 2 (A fixe à 2500×2500, B variable)**

`sousMat1` a un comportement non monotone : le temps baisse d'abord (n' = 100 à 1200),
puis remonte (n' = 1200 à 2000). Deux effets opposés se compensent : moins de positions de
départ possibles quand B grossit (A étant fixe), mais chaque vérification coûte de plus
en plus cher à mesure que B grandit. `sousMat2`, en revanche, croît régulièrement avec n' :
l'avantage du rejet rapide (premier/dernier élément de chaque ligne) s'amenuise quand B
devient grande, car une bonne partie de la comparaison complète finit par être nécessaire
de toute façon. L'écart entre les deux fonctions se réduit fortement aux grandes valeurs de
n' (environ ×20 à n' = 100, environ ×1,3 seulement à n' = 2000).

## Conclusion

**Partie A.** Le produit de matrices suit une complexité cubique en théorie (O(n³)),
confirmée qualitativement par l'expérience (exposant mesuré proche de 3, mais
systématiquement au-dessus). L'écart quantitatif croissant avec n s'explique par des
facteurs matériels (hiérarchie de cache) que le modèle théorique, qui ne compte que les
opérations arithmétiques, ne prend pas en compte.

**Partie B.** `sousMat2` est toujours au moins aussi rapide que `sousMat1`, mais son
avantage dépend fortement du contexte : il est considérable lorsque la sous-matrice
cherchée est petite par rapport à la matrice principale (cas le plus réaliste en pratique),
et se réduit nettement lorsque la sous-matrice approche la taille de la matrice où elle est
cherchée. La complexité théorique à deux paramètres (n, m, n', m') explique bien ce
comportement : selon lequel des deux facteurs domine, le temps mesuré peut croître,
décroître, ou même ne pas être monotone — ce qui est précisément observé dans
l'expérience 2.

**Remarque générale.** Dans ce TP, la complexité du produit de matrices dépend de trois
paramètres (n, m, p), et celle de la recherche de sous-matrice dépend de quatre
(n, m, n', m'). Contrairement à des algorithmes à un seul paramètre, l'effet de chacun doit
être isolé séparément pour comprendre le comportement global — d'où la nécessité des deux
expériences distinctes dans la partie B.

## Environnement

- Compilateur : MinGW GCC (Windows / CMD)
- Mesures : `clock()` (`time.h`), répétition adaptative pour stabiliser les temps courts
- Graphes : Python 3, `matplotlib`, `numpy`