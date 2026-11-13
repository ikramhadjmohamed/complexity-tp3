import csv
import numpy as np
import matplotlib.pyplot as plt

def lire_csv(nom_fichier):
    x_valeurs = []
    t1_valeurs = []
    t2_valeurs = []
    with open(nom_fichier, newline="") as f:
        lecteur = csv.reader(f, delimiter=";")
        next(lecteur)  # ignorer l'en-tete
        for ligne in lecteur:
            x_valeurs.append(int(ligne[0]))
            t1_valeurs.append(float(ligne[1]))
            t2_valeurs.append(float(ligne[2]))
    return x_valeurs, t1_valeurs, t2_valeurs

# --- Experience 1 : n, m varient ---
n_vals, t1_exp1, t2_exp1 = lire_csv("resultats_exp1.csv")

plt.figure()
plt.plot(n_vals, t1_exp1, marker="o", label="sousMat1")
plt.plot(n_vals, t2_exp1, marker="s", label="sousMat2")
plt.xlabel("n (taille de A, n'=m'=10 fixe)")
plt.ylabel("Temps d'execution (secondes)")
plt.title("Experience 1 : variation de n, m")
plt.legend()
plt.grid(True)
plt.savefig("graphe_exp1.png")

# --- Experience 2 : n', m' varient ---
np_vals, t1_exp2, t2_exp2 = lire_csv("resultats_exp2.csv")

plt.figure()
plt.plot(np_vals, t1_exp2, marker="o", label="sousMat1")
plt.plot(np_vals, t2_exp2, marker="s", label="sousMat2")
plt.xlabel("n' (taille de B, n=m=2500 fixe)")
plt.ylabel("Temps d'execution (secondes)")
plt.title("Experience 2 : variation de n', m'")
plt.legend()
plt.grid(True)
plt.savefig("graphe_exp2.png")

plt.show()

# --- Exposants experimentaux (comme pour l'exercice 1) ---
a1_exp1, _ = np.polyfit(np.log(n_vals), np.log(t1_exp1), 1)
a2_exp1, _ = np.polyfit(np.log(n_vals), np.log(t2_exp1), 1)
print(f"Exp1 - exposant sousMat1 : {a1_exp1:.3f}")
print(f"Exp1 - exposant sousMat2 : {a2_exp1:.3f}")

a1_exp2, _ = np.polyfit(np.log(np_vals), np.log(t1_exp2), 1)
a2_exp2, _ = np.polyfit(np.log(np_vals), np.log(t2_exp2), 1)
print(f"Exp2 - exposant sousMat1 : {a1_exp2:.3f}")
print(f"Exp2 - exposant sousMat2 : {a2_exp2:.3f}")