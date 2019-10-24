main_grille = [[0,0,0,0,0,0,0,0,0,0]]*10
rayon_comunication = 2
rayon_detection = 1

main_grille = [[0 for j in range(10)] for i in range(10)]

def liste_voisin_communiquant_grille(grille, x, y, rayon_comunication):

	liste_voisin_courant = []

	#gauche
	for i in range(1,rayon_comunication+1):
		if (0<= x <= 10 and 0<= y-i <= 10 and grille[x][y-i] == 0):
			liste_voisin_courant.append([x,y-i])

	#droite
	for i in range(1,rayon_comunication+1):
		if (0<= x <= 10 and 0<= y+i <= 10 and grille[x][y+i] == 0):
			liste_voisin_courant.append([x,y+i])

	#haut
	for i in range(1,rayon_comunication+1):
		if (0<= x+i <= 10 and 0<= y <= 10 and grille[x+i][y] == 0):
			liste_voisin_courant.append([x+i,y])

	#bas
	for i in range(1,rayon_comunication+1):
		if (0<= x-i <= 10 and 0<= y <= 10 and grille[x-i][y] == 0):
			liste_voisin_courant.append([x-i,y])

	#diag haut gauche
	for i in range(1,rayon_comunication):
		if (0<= x-i <= 10 and 0<= y+i <= 10 and grille[x-i][y+i] == 0):
			liste_voisin_courant.append([x-i,y+i])

	#diag haut droit
	for i in range(1,rayon_comunication):
		if (0<= x+i <= 10 and 0<= y+i <= 10 and grille[x+i][y+i] == 0):
			liste_voisin_courant.append([x+i,y+i])

	#diag haut bas
	for i in range(1,rayon_comunication):
		if (0<= x-i <= 10 and 0<= y-i <= 10 and grille[x-i][y-i] == 0):
			liste_voisin_courant.append([x-i,y-i])

	#diag haut haut
	for i in range(1,rayon_comunication):
		if (0<= x+i <= 10 and 0<= y-i <= 10 and grille[x+i][y-i] == 0):
			liste_voisin_courant.append([x+i,y-i])

	return liste_voisin_courant

# print(liste_voisin_communiquant_grille(main_grille,6,6,rayon_comunication))


def liste_voisin_detecte_grille(grille, x, y, rayon_detection):
	liste_voisin_courant = []

	#gauche
	for i in range(1,rayon_detection+1):
		if (0<= x <= 10 and 0<= y-i <= 10 and grille[x][y-i] == 0):
			liste_voisin_courant.append([x,y-i])
			print("gauche")

	#droite
	for i in range(1,rayon_detection+1):
		if (0<= x <= 10 and 0<= y+i <= 10 and grille[x][y+i] == 0):
			liste_voisin_courant.append([x,y+i])
			print("droite")

	#haut
	for i in range(1,rayon_detection+1):
		if (0<= x+i <= 10 and 0<= y <= 10 and grille[x+i][y] == 0):
			liste_voisin_courant.append([x+i,y])
			print("haut")

	#bas
	for i in range(1,rayon_detection+1):
		if (0<= x-i <= 10 and 0<= y <= 10 and grille[x-i][y] == 0):
			liste_voisin_courant.append([x-i,y])
			print("bas")

	#diag haut gauche
	for i in range(1,rayon_detection):
		if (0<= x-i <= 10 and 0<= y+i <= 10 and grille[x-i][y+i] == 0):
			liste_voisin_courant.append([x-i,y+i])
			print("diag haut gauche")

	#diag haut droit
	for i in range(1,rayon_detection):
		if (0<= x+i <= 10 and 0<= y+i <= 10 and grille[x+i][y+i] == 0):
			liste_voisin_courant.append([x+i,y+i])
			print("diag haut droit")

	#diag haut bas
	for i in range(1,rayon_detection):
		if (0<= x-i <= 10 and 0<= y-i <= 10 and grille[x-i][y-i] == 0):
			liste_voisin_courant.append([x-i,y-i])
			print("diag haut bas")

	#diag haut haut
	for i in range(1,rayon_detection):
		if (0<= x+i <= 10 and 0<= y-i <= 10 and grille[x+i][y-i] == 0):
			liste_voisin_courant.append([x+i,y-i])
			print("diag haut haut")

	return liste_voisin_courant


# print(liste_voisin_detecte_grille(main_grille,0,0,rayon_detection))
print(liste_voisin_communiquant_grille(main_grille,0,0,rayon_comunication))