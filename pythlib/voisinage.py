import numpy as np
import matplotlib.pyplot as plt
import random
from point import *
from dfs import *
from glouton import *

# Cette fonction retourne une liste de noeuds sur lesquels on peut rajouter une detecteur
def add_node_list(grille):
	rayon_communication = grille.rayon_communication
	solution_courante = grille.get_solution_courante()

	cible_list = []
	for capteur in solution_courante:
		capteur_pts = grille.get_pts(capteur)

		cibles_voisines = grille.recherche_voisin(capteur_pts,rayon_communication,Type.Cible)
		for cible in cibles_voisines:
			if cible not in cible_list:
				cible_list.append(cible)
	return cible_list

def add_node(grille, node):
	rayon_communication = grille.rayon_communication
	rayon_detection = grille.rayon_detection

	node_name = node.get_name()

	capteurs_voisins = grille.recherche_voisin(node,rayon_communication,Type.Capteur)
	cibles_voisines = grille.recherche_voisin(node,rayon_detection,Type.Cible)
	maj_grille(grille,capteurs_voisins,node,cibles_voisines,0)
	grille.add_node_solution_courante(node_name)

# Cette fonction retourne une liste de noeuds sur lesquels on peut enlever un detecteur
def remove_node_list(grille):
	rayon_detection = grille.rayon_detection

	capteur_list = []
	solution_courante = grille.get_solution_courante ()

	#On itère sur tout les capteurs
	for capteur in solution_courante:
		capteur_pts = grille.get_pts(capteur)
		# print('remove : ',capteur.name)
		capteur_valide=0
		#On regarde les cibles qu'il capte
		cibles_voisines = grille.recherche_voisin(capteur_pts,rayon_detection,Type.Cible)
		for cible in cibles_voisines:
			cible_pts = grille.get_pts(cible)
			cible_aux = cible_pts.get_aux()
			if len(cible_aux) <= 1:
				# print("on break pour : ",capteur)
				capteur_valide=1
				break

		if(capteur_valide==0):
			#On regarde si en l'enlevant le graphe reste connexe
			# print(run_dfs(grille.pts_list,[capteur.name]))
			dfs_result = len(run_dfs(grille.pts_list,[capteur]))
			# print(dfs_result,' pour ',capteur)
			if(dfs_result!=len(solution_courante)):
				capteur_valide=1

		if(capteur_valide==0):
			capteur_list.append(capteur)

	return capteur_list

def remove_node(grille, node):
	rayon_detection = grille.rayon_detection

	node_name = node.get_name()
	node_aux = node.get_aux()

	grille.remove_node_solution_courante(node_name)

	capteurs_voisins = node_aux
	for capteur in capteurs_voisins:
		grille.set_capteur_comm_aux_remove_one(capteur,node_name)

	cibles_voisines = grille.recherche_voisin(node,rayon_detection,Type.Cible)
	for cible in cibles_voisines:
		grille.set_cible_detect_aux_remove_one(cible,node_name)

	capteurs_voisins = grille.recherche_voisin(node,rayon_detection,Type.Capteur)
	grille.set_nouvelle_cible(node_name,capteurs_voisins)


# def remove_node

# def switch_node