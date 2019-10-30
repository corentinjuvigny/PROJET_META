from grille import *
from draw import *
from glouton import *
from readfile import *
from voisinage import *
from scipy import spatial

# Variables globales :
rayon_communication = 1
rayon_detection = 3
taille_grille = 5

#print_grille(taille_grille)

# print(liste_voisin_detecte_grille(main_grille,0,0,rayon_detection))
# print(liste_voisin_communiquant_grille(main_grille,0,0,rayon_communication))


points = read_file("../Instances/captANOR1500_21_500.dat")
points_list = points[0]
point_list_for_kd_tree = points[1]

kd_tree = spatial.KDTree(point_list_for_kd_tree)

###### IMPORTANT :

## AVEC FICHIER DE POINTS, CETTE LIGNE
# grille = Grille(taille_grille,[points_list,kd_tree])

## SANS FICHIER DE POINTS, CETTE LIGNE
grille = Grille(taille_grille,[])

# sprint_grid(grille)

def main(grille,rayon_communication,rayon_detection):
	res = construction_gloutonne_solution(grille,rayon_communication,rayon_detection)
	grille = res[0]
	liste_capteur_courant = res[1]
	# print(len(run_dfs(grille.pts_list)))
	# print(len(liste_capteur_courant))
	print("Add node list")
	print(add_node_list(grille,liste_capteur_courant,rayon_communication))
	print("Remove node list")
	print(grille)
	print(remove_node_list(grille,liste_capteur_courant,rayon_detection))
	add_node(grille,grille.pts_list[11],rayon_communication,rayon_detection,liste_capteur_courant)
	print(grille)
	print(liste_capteur_courant)
	print(remove_node_list(grille,liste_capteur_courant,rayon_detection))
	remove_node(grille,grille.pts_list[7],rayon_communication,rayon_detection,liste_capteur_courant)
	print(grille)
	print(liste_capteur_courant)
	print_grid(grille)


main(grille,rayon_communication,rayon_detection)