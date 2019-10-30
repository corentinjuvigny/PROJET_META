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
		pts_name = pts.name
		if (typ == Type.Puits):
			plt.scatter(pts_x, pts_y,marker='D',color='black')
			plt.text(pts_x+0.1, pts_y+0.1, pts_name, fontsize=8)
		elif (typ == Type.Capteur):
			plt.scatter(pts_x, pts_y,marker='o',color='black')
			for voisin_comm in aux:
				voisin_comm_data = pts_list[voisin_comm]
				plt.plot([pts_x,voisin_comm_data.x],[pts_y,voisin_comm_data.y], color='red',linewidth=5)
				plt.text(pts_x+0.1, pts_y+0.1, pts_name, fontsize=8)
		elif (typ == Type.Cible):
			plt.scatter(pts_x, pts_y,marker='o',color='grey')
			plt.text(pts_x+0.1, pts_y+0.1, pts_name, fontsize=8)
			color = "%06x" % np.random.randint(0, 0xFFFFFF)
			for voisin_detect in aux:
				voisin_detect_data = pts_list[voisin_detect]
				plt.plot([pts_x,voisin_detect_data.x],[pts_y,voisin_detect_data.y],linewidth=0.5,color="#"+color)
	plt.show()
