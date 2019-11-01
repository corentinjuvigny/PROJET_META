from draw import *
import numpy as np
from dfs import *

# Cette fonction retourne la meilleur cible dans le voisinage d'un capteur
def recherche_meilleur_cible_remplacer(grille,capteur_courant_pts,liste_cibles_couvertes):

	rayon_communication = grille.get_rayon_communication()
	rayon_detection = grille.get_rayon_detection()

	capteur_courant_name = capteur_courant_pts.get_name()

	#On cherche les cibles qui communiquent avec le capteur courant
	cibles_voisines = grille.recherche_voisin(capteur_courant_pts,rayon_communication,Type.Cible)

	voisin_choisi = None
	cibles_couvertes_par_voisin_choisi = []
	nb_nouvelles_cibles_couv_max = 0

	#On cherche le voisin qui detecte le plus de cibles
	for voisin in cibles_voisines:
		## print('#######################################')
		## print('On explore : (',voisin.x,',',voisin.y,')')
		voisin_pts = grille.get_pts(voisin)

		name = voisin_pts.name
		content = liste_cibles_couvertes.get(name)

		# Cette cible à déjà été checkée, on ne la rechecke pas
		# une deuxième fois ...
		if (content != None):
			## print(content)
			content.append(capteur_courant_name)
			## print('nouvelle_liste : ',liste_cibles_couvertes)
		else:
			liste_cibles_couvertes[name]=[capteur_courant_name]
			nb_nouvelles_cibles_couv = 0
			cibles_couvertes = grille.recherche_voisin(voisin_pts,rayon_detection,Type.Cible)
			for cible in cibles_couvertes:
				cible_pts = grille.get_pts(cible)
				## print('Il couvre : (',cible_pts.x,',',cible_pts.y,')')
				if (cible_pts.aux == []):
					## print("new")
					nb_nouvelles_cibles_couv+=1
			# Si la cible choisie pour y placer un capteur n'est pas déjà repérée,
			# au augmente la couverture de 1
			if (voisin_pts.aux == []):
				nb_nouvelles_cibles_couv+=1

			## print('On atteind : ',nb_nouvelles_cibles_couv," nouvelles cibles")
			## print('#######################################')
			if (nb_nouvelles_cibles_couv >= nb_nouvelles_cibles_couv_max):
				voisin_choisi = voisin_pts
				cibles_couvertes_par_voisin_choisi = cibles_couvertes
				nb_nouvelles_cibles_couv_max = nb_nouvelles_cibles_couv

	return (voisin_choisi,cibles_couvertes_par_voisin_choisi,nb_nouvelles_cibles_couv_max,liste_cibles_couvertes)

def maj_grille(grille,capteur_list,nouveau_capteur_pts,cibles_couvertes,nb_nouvelles_cibles_couv):

	nouveau_capteur_name = nouveau_capteur_pts.get_name()

	#Ajout d'un capteur sur le voisin choisi
	grille.set_nouveau_capteur(nouveau_capteur_name, capteur_list)

	grille.set_couverture_moins(nb_nouvelles_cibles_couv)

	for capteur_name in capteur_list:
		grille.set_capteur_comm_aux_add_one(capteur_name,nouveau_capteur_name)

	# Ajout du capteur dans la liste des voisins des cibles couvertes
	for cible_name in cibles_couvertes:
		grille.set_cible_detect_aux_add_one(cible_name,nouveau_capteur_name)

def construction_gloutonne_solution(grille):

	puits_pts = grille.get_pts(0)
	puits_name = puits_pts.get_name()

	# On cherche les cibles qui communiquent avec le capteur initial,
	# donc le puits.
	meilleur_cible = recherche_meilleur_cible_remplacer(grille, puits_pts, {})
	nouveau_capteur_pts = meilleur_cible[0]
	cibles_couvertes = meilleur_cible[1]
	nb_nouvelles_cibles_couv = meilleur_cible[2]
	print("TEST : nb_nouvelles_cibles_couv\n",nb_nouvelles_cibles_couv)
	print("On choisit : ")
	print(nouveau_capteur_pts)
	grille.add_node_solution_courante(nouveau_capteur_pts.get_name())

	## print('&&&&&&&&&&&&&&&&&&&&&&&&&&& On vient de : (',puits.x,',',puits.y,')')
	## print('&&&&&&&&&&&&&&&&&&&&&&&&&&& On rajoute : (',nouveau_capteur_pts.x,',',nouveau_capteur_pts.y,')')

	maj_grille(grille,[puits_name],nouveau_capteur_pts,cibles_couvertes,nb_nouvelles_cibles_couv)

	# print_grid(taille_grille, grille)

	while(grille.couverture > 0):
		print(grille.couverture)
		meilleur_voisin_choisi_pts = None
		cibles_couvertes_par_voisin_choisi = []
		nb_nouvelles_cibles_couv_max = 0
		liste_cibles_couvertes = {}

		#On itère sur tout les capteur de la grille pour trouver celui qui communique avec la meilleure cible
		solution_courante = grille.get_solution_courante()
		for capteur_name in solution_courante:
			capteur_pts = grille.get_pts(capteur_name)
			## print('&&&&&&&&&&&&&&&&&&&&&&&&&&& On test de : (',capteur_pts.get_x(),',',capteur_pts.get_y(),')')

			meilleur_cible = recherche_meilleur_cible_remplacer(grille,capteur_pts,liste_cibles_couvertes)

			capteur_courant_pts = meilleur_cible[0]
			cibles_couvertes = meilleur_cible[1]
			nb_nouvelles_cibles_couv = meilleur_cible[2]
			liste_cibles_couvertes = meilleur_cible[3]

			if (nb_nouvelles_cibles_couv > nb_nouvelles_cibles_couv_max):
				meilleur_voisin_choisi_pts = capteur_courant_pts
				cibles_couvertes_par_voisin_choisi = cibles_couvertes
				nb_nouvelles_cibles_couv_max = nb_nouvelles_cibles_couv
				## print("maj best : ",meilleur_voisin_choisi_pts.x,',',meilleur_voisin_choisi_pts.y)

		## print('&&&&&&&&&&&&&&&&&&&&&&&&&&& On rajoute : ',meilleur_voisin_choisi_pts.name,'(',meilleur_voisin_choisi_pts.x,',',meilleur_voisin_choisi_pts.y,')')
		## print('Il est capté par :')
		## print(liste_cibles_couvertes)
		## print(liste_cibles_couvertes[meilleur_voisin_choisi_pts.name])
		## print('#############""')
		grille.add_node_solution_courante(meilleur_voisin_choisi_pts.get_name())
		## print('On vient de : (',capteur.x,',',capteur.y,')')
		## print('On rajoute : (',meilleur_voisin_choisi_pts.x,',',meilleur_voisin_choisi_pts.y,')')
		maj_grille(grille,liste_cibles_couvertes[meilleur_voisin_choisi_pts.name],meilleur_voisin_choisi_pts,cibles_couvertes_par_voisin_choisi,nb_nouvelles_cibles_couv_max)

	return grille