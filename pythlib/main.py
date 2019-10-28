from grille import *

main_grille = [[0,0,0,0,0,0,0,0,0,0]]*10

main_grille = [[0 for j in range(10)] for i in range(10)]


# print(liste_voisin_detecte_grille(main_grille,0,0,rayon_detection))
# print(liste_voisin_communiquant_grille(main_grille,0,0,rayon_comunication))


grille = Grille()
point_random = grille.grille[2][2]
grille.voisin_point(point_random,0)
