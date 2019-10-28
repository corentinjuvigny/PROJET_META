from point import *

rayon_comunication = 2
rayon_detection = 1

def voisin_point_append(voisin_pos_x, voisin_pos_y, grille, liste_voisin_courant, desc):
	if (0<=voisin_pos_x<=10 and 0<=voisin_pos_y<=10):
		point = grille[voisin_pos_x][voisin_pos_y]
		if (point.typ == Type.Cible):
			liste_voisin_courant.append(point)
			print(point)
			print(desc)

	return liste_voisin_courant

class Grille:
	def __init__(self):
		n = 10
		self.grille = [[Point(x,y,Type.Cible,[]) for y in range(n)] for x in range(n)]
		self.grille[0][0] = Point(0,0,Type.Puits,[])

	def __repr__(self):
		l = []
		for ligne in self.grille:
			v = []
			for e in ligne:
				v.append([str(e)])
			l.append(str(v))
		return str('\n'.join(l))

	def voisin_point(self,point,voisin_type):
		rayon = 0

		if (voisin_type == 0):
			rayon = rayon_comunication
		else:
			rayon = rayon_detection

		liste_voisin_courant = []
		pos_x = point.x
		pos_y = point.y

		#gauche
		for i in range(1,rayon+1):
			voisin_pos_x = pos_x
			voisin_pos_y = pos_y-i
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self.grille,liste_voisin_courant,"bas")

		#droite
		for i in range(1,rayon+1):
			voisin_pos_x = pos_x
			voisin_pos_y = pos_y+i
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self.grille,liste_voisin_courant,"haut")
		#haut
		for i in range(1,rayon+1):
			voisin_pos_x = pos_x+i
			voisin_pos_y = pos_y
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self.grille,liste_voisin_courant,"droit")
		#bas
		for i in range(1,rayon+1):
			voisin_pos_x = pos_x-i
			voisin_pos_y = pos_y
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self.grille,liste_voisin_courant,"gauche")

		#diag haut gauche
		for i in range(1,rayon):
			voisin_pos_x = pos_x-i
			voisin_pos_y = pos_y+i
			point_haut_gauche = self.grille[pos_x-i][pos_y+i]
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self.grille,liste_voisin_courant,"bas gauche")

		#diag haut droit
		for i in range(1,rayon):
			voisin_pos_x = pos_x+i
			voisin_pos_y = pos_y+i
			point_haut_droit = self.grille[pos_x+i][pos_y+i]
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self.grille,liste_voisin_courant,"bas droit")

		#diag bas gauche
		for i in range(1,rayon):
			voisin_pos_x = pos_x-i
			voisin_pos_y = pos_y-i
			point_bas_gauche = self.grille[pos_x-i][pos_y-i]
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self.grille,liste_voisin_courant,"haut gauche")

		#diag bas droit
		for i in range(1,rayon):
			voisin_pos_x = pos_x+i
			voisin_pos_y = pos_y-i
			point_bas_droit = self.grille[pos_x+i][pos_y-i]
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self.grille,liste_voisin_courant,"haut droit")
		return liste_voisin_courant

