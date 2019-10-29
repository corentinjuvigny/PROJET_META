from point import *

def voisin_point_append(voisin_pos_x, voisin_pos_y, grille_obj, liste_voisin_courant, desc):

	taille_grille = grille_obj.taille
	grille = grille_obj.grille

	if (0<=voisin_pos_x<=taille_grille-1 and 0<=voisin_pos_y<=taille_grille-1):
		point = grille[voisin_pos_x][voisin_pos_y]
		if (point.typ == Type.Cible):
			liste_voisin_courant.append(point)
			# print(point)
			# print(desc)

	return liste_voisin_courant

class Grille:
	def __init__(self,taille):
		n = taille
		self.grille = [[Point(str(x)+","+str(y),x,y,Type.Cible,[]) for y in range(n)] for x in range(n)]
		self.grille[0][0] = Point('0,0',0,0,Type.Puits,[])
		self.couverture = 1
		self.taille = n

	def __repr__(self):
		l = []
		for ligne in self.grille:
			v = []
			for e in ligne:
				v.append([str(e)])
			l.append(str(v))
		return str('Couverture :'+str(self.couverture)+'\n'.join(l))

	def set_capteur(self,x,y):
		self.grille[x][y].typ = Type.Capteur
		self.grille[x][y].aux = []

	def set_couverture(self,c):
		self.couverture += c

	def set_capteur_communication(self,x,y,voisin):
		self.grille[x][y].aux.append({'x':voisin.x,'y':voisin.y})

	def set_cible_voisin(self,x,y,voisin):
		self.grille[x][y].aux.append({'x':voisin.x,'y':voisin.y})

	def voisin_point(self,point,rayon):
		rayon = rayon

		liste_voisin_courant = []
		pos_x = point.x
		pos_y = point.y

		#gauche
		for i in range(1,rayon+1):
			voisin_pos_x = pos_x
			voisin_pos_y = pos_y-i
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self,liste_voisin_courant,"haut")

		#droite
		for i in range(1,rayon+1):
			voisin_pos_x = pos_x
			voisin_pos_y = pos_y+i
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self,liste_voisin_courant,"bas")
		#haut
		for i in range(1,rayon+1):
			voisin_pos_x = pos_x+i
			voisin_pos_y = pos_y
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self,liste_voisin_courant,"droit")
		#bas
		for i in range(1,rayon+1):
			voisin_pos_x = pos_x-i
			voisin_pos_y = pos_y
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self,liste_voisin_courant,"gauche")

		#diag haut gauche
		for i in range(1,rayon):
			voisin_pos_x = pos_x-i
			voisin_pos_y = pos_y+i
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self,liste_voisin_courant,"haut gauche")

		#diag haut droit
		for i in range(1,rayon):
			voisin_pos_x = pos_x+i
			voisin_pos_y = pos_y+i
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self,liste_voisin_courant,"haut droit")

		#diag bas gauche
		for i in range(1,rayon):
			voisin_pos_x = pos_x-i
			voisin_pos_y = pos_y-i
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self,liste_voisin_courant,"bas gauche")

		#diag bas droit
		for i in range(1,rayon):
			voisin_pos_x = pos_x+i
			voisin_pos_y = pos_y-i
			liste_voisin_courant = voisin_point_append(voisin_pos_x,voisin_pos_y,self,liste_voisin_courant,"bas droit")
		return liste_voisin_courant

