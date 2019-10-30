import numpy as np
import matplotlib.pyplot as plt
import random
from point import *
from dfs import *
from glouton import *

# Cette fonction retourne une liste de noeuds sur lesquels on peut rajouter une detecteur
def add_node_list(grille,liste_capteur_courant,rayon_communication):
	cible_list = []
	for capteur in liste_capteur_courant:
		cibles_voisines = grille.recherche_voisin(capteur,rayon_communication,Type.Cible)
		for cible in cibles_voisines:
			if cible.name not in cible_list:
				cible_list.append(cible.name)
	return cible_list

def add_node(grille, node, rayon_communication,rayon_detection,liste_capteur_courant):
	capteurs_voisins = grille.recherche_voisin_2(node,rayon_communication,Type.Capteur)
	cibles_voisines = grille.recherche_voisin(node,rayon_detection,Type.Cible)
	maj_grille(grille,capteurs_voisins,node,cibles_voisines,0)
	liste_capteur_courant.append(node)

# Cette fonction retourne une liste de noeuds sur lesquels on peut enlever un detecteur
def remove_node_list(grille,liste_capteur_courant,rayon_detection):
	capteur_list = []
	#On itère sur tout les capteurs
	for capteur in liste_capteur_courant:
		# print('remove : ',capteur.name)
		capteur_valide=0
		#On regarde les cibles qu'il capte
		cibles_voisines = grille.recherche_voisin(capteur,rayon_detection,Type.Cible)
		for cible in cibles_voisines:
			if len(cible.aux) <= 1:
				print("on break pour : ",capteur.name)
				capteur_valide=1
				break

		if(capteur_valide==0):
			#On regarde si en l'enlevant le graphe reste connexe
			# print(run_dfs(grille.pts_list,[capteur.name]))
			dfs_result = len(run_dfs(grille.pts_list,[capteur.name]))
			print(dfs_result,' pour ',capteur.name)
			if(dfs_result!=len(liste_capteur_courant)):
				capteur_valide=1

		if(capteur_valide==0):
			capteur_list.append(capteur.name)

	return capteur_list

def remove_node(grille, node, rayon_communication, rayon_detection,liste_capteur_courant):
	liste_capteur_courant.remove(node)

	capteurs_voisins = node.aux
	for capteur in capteurs_voisins:
		grille.set_capteur_communication_remove(capteur,node)

	cibles_voisines = grille.recherche_voisin(node,rayon_detection,Type.Cible)
	for cible in cibles_voisines:
		grille.set_cible_voisin_remove(cible,node)

	capteurs_voisins = grille.recherche_voisin_2(node,rayon_detection,Type.Capteur)
	grille.set_cible(node,capteurs_voisins)


# def remove_node

# def switch_node