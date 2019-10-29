import numpy as np
import matplotlib.pyplot as plt
import random
from point import *

def print_grid(grille):
	pts_list = grille.pts_list
	taille_grille = grille.taille
	plt.axis([0, taille_grille, 0, taille_grille])
	plt.xticks(np.arange(-1, taille_grille+1, 1))
	plt.yticks(np.arange(-1, taille_grille+1, 1))
	plt.gca().invert_yaxis()
	plt.grid(True,linewidth=0.25)

	for pts in pts_list:
		typ = pts.typ
		aux = pts.aux
		pts_x = pts.x
		pts_y = pts.y
		if (typ == Type.Puits):
			plt.scatter(pts_x, pts_y,marker='D',color='black')
		elif (typ == Type.Capteur):
			plt.scatter(pts_x, pts_y,marker='o',color='black')
			for voisin_comm in aux:
				# print(i, j)
				# print(voisin_comm)
				plt.plot([pts_x,voisin_comm['x']],[pts_y,voisin_comm['y']], color='red',linewidth=5)
		elif (typ == Type.Cible):
			plt.scatter(pts_x, pts_y,marker='o',color='grey')
			color = "%06x" % np.random.randint(0, 0xFFFFFF)
			for voisin_detect in aux:
				# print(voisin_detect)
				plt.plot([pts_x,voisin_detect['x']],[pts_y,voisin_detect['y']],linewidth=0.5,color="#"+color)
	plt.show()
