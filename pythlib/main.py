from grille import *
from draw import *
from glouton import *

# Variables globales :
rayon_communication = 3
rayon_detection = 4
taille_grille = 5
nombre_points = taille_grille*taille_grille
main_grille = [[0 for j in range(taille_grille)] for i in range(taille_grille)]

#print_grille(taille_grille)

# print(liste_voisin_detecte_grille(main_grille,0,0,rayon_detection))
# print(liste_voisin_communiquant_grille(main_grille,0,0,rayon_communication))


grille = Grille(taille_grille)

# print_grid(taille_grille,grille)

construction_gloutonne_solution(grille,rayon_communication,rayon_detection,nombre_points)


