from draw import *
import numpy as np

# Cette fonction retourne la meilleur cible dans le voisinage d'un capteur
def recherche_meilleur_cible_remplacer(grille,capteur_courant,rayon_communication,rayon_detection):

	#On cherche les cibles qui communiquent avec le capteur courant
	cibles_voisines = grille.voisin_point(capteur_courant,rayon_communication)
	voisin_choisi = None
	cibles_couvertes_par_voisin_choisi = []
	nb_nouvelles_cibles_couv_max = 0

	#On cherche le voisin qui detecte le plus de cibles
	for voisin in cibles_voisines:
		print('#######################################')
		print('On explore : (',voisin.x,',',voisin.y,')')
		nb_nouvelles_cibles_couv = 0
		cibles_couvertes = grille.voisin_point(voisin,rayon_detection)
		for cible in cibles_couvertes:
			print('Il couvre : (',cible.x,',',cible.y,')')
			if (cible.aux == []):
				print("new")
				nb_nouvelles_cibles_couv+=1
		# Si la cible choisie pour y placer un capteur n'est pas déjà repérée,
		# au augmente la couverture de 1
		if (voisin.aux == []):
			nb_nouvelles_cibles_couv+=1

		print('On atteind : ',nb_nouvelles_cibles_couv," nouvelles cibles")
		print('#######################################')
		if (nb_nouvelles_cibles_couv >= nb_nouvelles_cibles_couv_max):
			voisin_choisi = voisin
			cibles_couvertes_par_voisin_choisi = cibles_couvertes
			nb_nouvelles_cibles_couv_max = nb_nouvelles_cibles_couv

	return (voisin_choisi,cibles_couvertes_par_voisin_choisi,nb_nouvelles_cibles_couv_max)

def maj_grille(grille,capteur,voisin_choisi,cibles_couvertes,nb_nouvelles_cibles_couv):
	#Ajout d'un capteur sur le voisin choisi
	grille.set_capteur(voisin_choisi.x,voisin_choisi.y)
	grille.set_capteur_communication(voisin_choisi.x,voisin_choisi.y,capteur)
	grille.set_couverture(nb_nouvelles_cibles_couv)

	#Ajout du capteur dans la liste des voisins des cibles couvertes
	for cible in cibles_couvertes:
		grille.set_cible_voisin(cible.x,cible.y,voisin_choisi)

def construction_gloutonne_solution(grille,rayon_communication,rayon_detection,nombre_points):
	puits = grille.grille[0][0]
	liste_capteur_courant = []

	#On cherche les cibles qui communiquent avec le capteur initial,
	#donc le puits.
	meilleur_cible = recherche_meilleur_cible_remplacer(grille,puits,rayon_communication,rayon_detection)
	voisin_choisi = meilleur_cible[0]
	cibles_couvertes = meilleur_cible[1]
	nb_nouvelles_cibles_couv = meilleur_cible[2]

	liste_capteur_courant.append(voisin_choisi)

	print('&&&&&&&&&&&&&&&&&&&&&&&&&&& On vient de : (',puits.x,',',puits.y,')')
	print('&&&&&&&&&&&&&&&&&&&&&&&&&&& On rajoute : (',voisin_choisi.x,',',voisin_choisi.y,')')

	maj_grille(grille,puits,voisin_choisi,cibles_couvertes,nb_nouvelles_cibles_couv)

	#Ajout d'un capteur sur le voisin choisi
	# print_grid(taille_grille, grille)

	while(grille.couverture < nombre_points):
		# print(grille.couverture)
		meilleur_voisin_choisi = None
		meilleur_capteur_choisi = None
		cibles_couvertes_par_voisin_choisi = []
		nb_nouvelles_cibles_couv_max = 0

		#On itère sur tout les capteur de la grille pour trouver celui qui communique avec la meilleure cible
		for capteur in liste_capteur_courant:
			print('&&&&&&&&&&&&&&&&&&&&&&&&&&& On test de : (',capteur.x,',',capteur.y,')')
			meilleur_cible = recherche_meilleur_cible_remplacer(grille,capteur,rayon_communication,rayon_detection)
			voisin_choisi = meilleur_cible[0]
			cibles_couvertes = meilleur_cible[1]
			nb_nouvelles_cibles_couv = meilleur_cible[2]
			if (nb_nouvelles_cibles_couv > nb_nouvelles_cibles_couv_max):
				meilleur_voisin_choisi = voisin_choisi
				cibles_couvertes_par_voisin_choisi = cibles_couvertes
				nb_nouvelles_cibles_couv_max = nb_nouvelles_cibles_couv
				meilleur_capteur_choisi = capteur
				print("maj best : ",meilleur_voisin_choisi.x,',',meilleur_voisin_choisi.y)

		print('&&&&&&&&&&&&&&&&&&&&&&&&&&& On vient de : (',meilleur_capteur_choisi.x,',',meilleur_capteur_choisi.y,')')
		print('&&&&&&&&&&&&&&&&&&&&&&&&&&& On rajoute : (',meilleur_voisin_choisi.x,',',meilleur_voisin_choisi.y,')')
		liste_capteur_courant.append(meilleur_voisin_choisi)
		# print('On vient de : (',capteur.x,',',capteur.y,')')
		# print('On rajoute : (',meilleur_voisin_choisi.x,',',meilleur_voisin_choisi.y,')')
		maj_grille(grille,meilleur_capteur_choisi,meilleur_voisin_choisi,cibles_couvertes_par_voisin_choisi,nb_nouvelles_cibles_couv_max)
	print_grid(grille)
	print(grille)
	print(len(liste_capteur_courant))