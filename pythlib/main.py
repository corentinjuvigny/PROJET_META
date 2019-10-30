from grille import *
from draw import *
from glouton import *
from readfile import *
from voisinage import *
from recuit import *
from scipy import spatial

# Variables globales :
rayon_communication = 2
rayon_detection = 1
taille_grille = 10

#print_grille(taille_grille)

# print(liste_voisin_detecte_grille(main_grille,0,0,rayon_detection))
# print(liste_voisin_communiquant_grille(main_grille,0,0,rayon_communication))


points = read_file("../Instances/captANOR225_9_20.dat")
points_list = points[0]
point_list_for_kd_tree = points[1]

kd_tree = spatial.KDTree(point_list_for_kd_tree)

###### IMPORTANT :

## AVEC FICHIER DE POINTS, CETTE LIGNE
grille = Grille(taille_grille,[points_list,kd_tree],rayon_detection,rayon_communication)

## SANS FICHIER DE POINTS, CETTE LIGNE
# grille = Grille(taille_grille,[],rayon_detection,rayon_communication)

# sprint_grid(grille)

def main(grille):
	res = construction_gloutonne_solution(grille)
	grille = res

	add_node(grille,grille.pts_list[11])
	# print_grid(grille)
	print("Solution actuelle :",len(grille.get_solution_courante()))
	solution_min = recuit_simule(grille)
	print("Solution après recuit :",len(solution_min.get_solution_courante()))
	# print("Add node list")
	# print(add_node_list(grille,liste_capteur_courant))
	# print("Remove node list")
	# print(grille)
	# print(remove_node_list(grille,liste_capteur_courant))

	# print(liste_capteur_courant)
	# print(remove_node_list(grille,liste_capteur_courant))
	# remove_node(grille,grille.pts_list[7],liste_capteur_courant)
	# print(solution_min)
	# print(solution_min.get_solution_courante())
	print_grid(solution_min)
	plt.pause(10)

main(grille)