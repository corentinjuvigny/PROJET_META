from point import *

def voisin_point_append(voisin_pos_x, voisin_pos_y, grille_obj, liste_voisin_courant):

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
		taille_grille = self.taille
		liste_voisin_courant = []
		pos_x = point.x
		pos_y = point.y
		dist_pts = 0
		rayon_carre = pow(rayon,2)
		for x in range(0,taille_grille):
			for y in range(0,taille_grille):
				if (x != pos_x or y != pos_y):
					dist_pts = pow(x-pos_x,2) + pow(y-pos_y,2)
					# print(rayon_carre,'et',dist_pts)
					if (dist_pts <= rayon_carre):
						liste_voisin_courant = voisin_point_append(x,y,self,liste_voisin_courant)

		return liste_voisin_courant

