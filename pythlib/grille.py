from point import *
from scipy import spatial
import numpy as np

def create_grille(taille):
	pts_list = []
	pts_list_for_kd_tree = []
	k=0
	for i in range(0,taille):
		for j in range(0,taille):
			pts_list.append(Point(k,float(i),float(j),Type.Cible,[]))
			pts_list_for_kd_tree.append([float(i),float(j)])
			k+=1
	pts_list[0].typ = Type.Puits

	kd_tree = spatial.KDTree(pts_list_for_kd_tree)

	return pts_list,kd_tree

class Grille:
	def __init__(self,taille,content):
		if content == []:
			nouvelle_grille = create_grille(taille)
			self.pts_list = nouvelle_grille[0]
			self.kd_tree = nouvelle_grille[1]
		else:
			self.pts_list = content[0]
			self.kd_tree = content[1]

		self.couverture = len(self.pts_list) - 1
		self.taille = taille

	def __repr__(self):
		l = []
		for pts in self.pts_list:
			l.append(str(pts))
		return str('Couverture :'+str(self.couverture))+'\n'.join(l)

	def set_capteur(self,pts):
		self.pts_list[pts.name].typ = Type.Capteur
		self.pts_list[pts.name].aux = []

	def set_couverture(self,c):
		self.couverture -= c

	def set_capteur_communication(self,voisin,capteur):
		self.pts_list[voisin.name].aux.append({'x':capteur.x,'y':capteur.y})

	def set_cible_voisin(self,cible,voisin):
		self.pts_list[cible.name].aux.append({'x':voisin.x,'y':voisin.y})

	def voisin_point(self,point,rayon):
		kd_tree = self.kd_tree
		pts_list = self.pts_list

		liste_voisin_courant = []

		pos_x = point.x
		pos_y = point.y

		indice_voisins = kd_tree.query_ball_point([pos_x, pos_y], rayon)

		for i in indice_voisins:
			voisin = pts_list[i]
			x = voisin.x
			y = voisin.y
			typ = voisin.typ
			if ((x != pos_x or y != pos_y) and typ == Type.Cible):
					liste_voisin_courant.append(voisin)

		return liste_voisin_courant

