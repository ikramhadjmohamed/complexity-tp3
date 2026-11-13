import csv
import numpy as np
import matplotlib.pyplot as plt

n_valeurs = []
temps_valeurs = []

with open("resultats_produit.csv", newline="") as f:
    lecteur = csv.reader(f, delimiter=";")
    next(lecteur)
    for ligne in lecteur:
        n_valeurs.append(int(ligne[0]))
        temps_valeurs.append(float(ligne[1]))

# Garder seulement n >= 400
filtres = [(n, t) for n, t in zip(n_valeurs, temps_valeurs) if n >= 400]
n_f = [x[0] for x in filtres]
t_f = [x[1] for x in filtres]

a2, _ = np.polyfit(np.log(n_f), np.log(t_f), 1)
print(f"Exposant sur n >= 400 : {a2:.3f}")

# --- Estimation de la complexite experimentale ---
log_n = np.log(n_valeurs)
log_t = np.log(temps_valeurs)

a, log_c = np.polyfit(log_n, log_t, 1)
print(f"Exposant experimental a = {a:.3f}")
print(f"(theorie attendue : a = 3 pour O(n^3))")

# --- Graphe (comme avant) ---
plt.plot(n_valeurs, temps_valeurs, marker="o")
plt.xlabel("n (taille des matrices)")
plt.ylabel("Temps d'execution T (secondes)")
plt.title("Temps du produit de deux matrices carrees n x n")
plt.grid(True)
plt.savefig("graphe_produit.png")
plt.show()