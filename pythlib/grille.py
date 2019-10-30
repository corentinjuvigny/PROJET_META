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
	def __init__(self,taille,content,rayon_detection,rayon_communication):
		if content == []:
			nouvelle_grille = create_grille(taille)
			self.pts_list = nouvelle_grille[0]
			self.kd_tree = nouvelle_grille[1]
		else:
			self.pts_list = content[0]
			self.kd_tree = content[1]

		self.solution_courante = []
		self.couverture = len(self.pts_list)-1
		self.taille = taille
		self.rayon_detection = rayon_detection
		self.rayon_communication = rayon_communication

	def __repr__(self):
		l = []
		for pts in self.pts_list:
			l.append(str(pts))
		return str('Couverture :'+str(self.couverture))+'\n'.join(l)

	def get_pts_list(self):
		return self.pts_list

	def set_pts_list(self,pts_list):
		self.pts_list = pts_list


	def get_pts(self,i):
		return self.pts_list[i]

	def set_pts(self,i,pts):
		self.pts_list[i] = pts


	def get_kd_tree(self):
		return self.kd_tree

	def set_kd_tree(self,kd_tree):
		self.kd_tree = kd_tree

	def get_solution_courante(self):
		return self.solution_courante

	def set_solution_courante(self,solution_courante):
		self.solution_courante = solution_courante

	def add_node_solution_courante(self,node):
		self.solution_courante.append(node)

	def remove_node_solution_courante(self,node):
		self.solution_courante.remove(node)

	def get_couverture(self):
		return self.couverture

	def set_couverture(self,couverture):
		self.couverture = couverture

	def set_couverture_plus(self,c):
		self.couverture += c

	def set_couverture_moins(self,c):
		self.couverture -= c

	def get_taille(self):
		return self.taille

	def set_taille(self,taille):
		self.taille = taille


	def get_rayon_detection(self):
		return self.rayon_detection

	def set_rayon_detection(self,rayon_detection):
		self.rayon_detection = rayon_detection


	def get_rayon_communication(self):
		return self.rayon_communication

	def set_rayon_communication(self,rayon_communication):
		self.rayon_communication = rayon_communication


	def set_nouveau_capteur(self,capteur,capteur_list):
		self.pts_list[capteur].typ = Type.Capteur
		self.pts_list[capteur].aux = capteur_list

	def set_nouvelle_cible(self,cible,capteur_list):
		self.pts_list[cible].typ = Type.Cible
		self.pts_list[cible].aux = capteur_list

	def set_capteur_comm_aux_add_one(self,capteur,nouveau_capteur):
		self.pts_list[capteur].aux.append(nouveau_capteur)

	def set_capteur_comm_aux_remove_one(self,capteur,nouveau_capteur):
		self.pts_list[capteur].aux.remove(nouveau_capteur)

	def set_cible_detect_aux_add_one(self,cible,nouveau_capteur):
		self.pts_list[cible].aux.append(nouveau_capteur)

	def set_cible_detect_aux_remove_one(self,cible,nouveau_capteur):
		self.pts_list[cible].aux.remove(nouveau_capteur)

	def recherche_voisin(self,point,rayon,voisin_type):
		kd_tree = self.get_kd_tree()
		pts_list = self.get_pts_list()

		liste_voisin_courant = []

		pos_x = point.get_x()
		pos_y = point.get_y()

		indice_voisins = kd_tree.query_ball_point([pos_x, pos_y], rayon)

		for i in indice_voisins:
			voisin = pts_list[i]
			x = voisin.x
			y = voisin.y
			typ = voisin.typ
			if ((x != pos_x or y != pos_y) and typ == voisin_type):
					liste_voisin_courant.append(voisin.name)

		return liste_voisin_courant
