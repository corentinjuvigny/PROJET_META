from voisinage import *
from recuit import *
from grille import *
from copy import deepcopy
from draw import *
import random
import math

def tirage_aleatoire_voisinage(voisinage_add,voisinage_remove):
	choix_voisinage = random.randint(0,1)
	if ((choix_voisinage or not voisinage_remove) and voisinage_add ):
		choix_voisinage = 1
		node_name = random.choice(voisinage_add)

	elif(voisinage_remove):
		choix_voisinage = 0
		node_name = random.choice(voisinage_remove)
	else:
		choix_voisinage = -1
		node_name = -1

	return (choix_voisinage,node_name)

def solution_voisinage_aleatoire(grille_copy):
	voisinage_add = add_node_list(grille_copy)
	voisinage_remove = remove_node_list(grille_copy)
	solution = tirage_aleatoire_voisinage(voisinage_add,voisinage_remove)
	choix_voisinage = solution[0]
	node_name = solution[1]
	node_pts = grille_copy.get_pts(node_name)

	if(choix_voisinage==1):
		# print("On ajoute ",node_name)
		add_node(grille_copy,node_pts)
		choix_voisinage = random.randint(0,1)
		if(choix_voisinage and False):
			voisinage_2_add = add_node_list(grille_copy)
			node_name_2 = random.choice(voisinage_2_add)
			node_pts_2 = grille_copy.get_pts(node_name_2)
			add_node(grille_copy,node_pts_2)

	elif(choix_voisinage==0):
		# print("On retire ",node_name)
		remove_node(grille_copy,node_pts)

def energie_proba_distrib(delta_E,T):
	return math.exp(-delta_E/T)

def recuit_simule(grille):

	phi = 0.995 #Coefficient de diminution de T
	r = 2
	T_initial = 30
	critere_arret = 1000000
	print(phi,r,T_initial,critere_arret)
	random_float = 0
	energie_proba = 0
	voisinage_add = []
	voisinage_remove = []

	solution_initiale = grille
	solution_min = solution_initiale
	solution_courante = solution_initiale
	T = T_initial

	while (T>0.001):
		print("T : ",T,'/ best : ',len(solution_min.get_solution_courante()),'/ current',len(solution_courante.get_solution_courante()))

		for nb_iter in range(0,r):
			# print(solution_courante)
			print_grid(solution_courante)

			solution_voisin = deepcopy(solution_courante)
			solution_voisinage_aleatoire(solution_voisin)
			f_x = len(solution_courante.get_solution_courante())
			f_x_p = len(solution_voisin.get_solution_courante())
			delta_E = 10*(f_x_p - f_x)
			print(delta_E)
			if (delta_E <= 0):
				solution_courante = solution_voisin
				f_x = f_x_p
				f_x_min = len(solution_min.solution_courante)
				if (f_x < f_x_min):
					# print("Nouvelle solution opti : ",len(solution_courante.get_solution_courante()))
					solution_min = solution_courante
			else:
				random_float = random.uniform(0,1)
				energie_proba = energie_proba_distrib(delta_E,T)
				print(energie_proba)
				if (random_float <= energie_proba):
					solution_courante = solution_voisin
					# print("On degrade ! : ",len(solution_courante.get_solution_courante()))
		T = T * phi
		critere_arret -= 1


	return solution_min

