from grille import *
from draw import *
from glouton import *
from readfile import *
from scipy import spatial

# Variables globales :
rayon_communication = 1
rayon_detection = 4
taille_grille = 15
nombre_points = taille_grille*taille_grille

#print_grille(taille_grille)

# print(liste_voisin_detecte_grille(main_grille,0,0,rayon_detection))
# print(liste_voisin_communiquant_grille(main_grille,0,0,rayon_communication))


points = read_file("../Instances/InstancesTrunc/captTRUNC351_20_20.dat")
points_list = points[0]
point_list_for_kd_tree = points[1]

kd_tree = spatial.KDTree(point_list_for_kd_tree)

###### IMPORTANT :

## AVEC FICHIER DE POINTS, CETTE LIGNE
# grille = Grille(taille_grille,[points_list,kd_tree])

## SANS FICHIER DE POINTS, CETTE LIGNE
grille = Grille(taille_grille,[])

construction_gloutonne_solution(grille,rayon_communication,rayon_detection,nombre_points)


