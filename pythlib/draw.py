import numpy as np
import matplotlib.pyplot as plt
import random
from point import *

def print_grid(grille):
	taille_grille = grille.taille
	plt.axis([0, taille_grille, 0, taille_grille])
	plt.xticks(np.arange(-1, taille_grille+1, 1))
	plt.yticks(np.arange(-1, taille_grille+1, 1))
	plt.gca().invert_yaxis()
	plt.grid(True,linewidth=0.25)

	for i in range(taille_grille):
		for j in range(taille_grille):
			pts = grille.grille[i][j]
			typ = pts.typ
			aux = pts.aux
			pts_x = pts.x
			pts_y = pts.y
			if (typ == Type.Puits):
				plt.scatter(i, j,marker='D',color='black')
			elif (typ == Type.Capteur):
				plt.scatter(i, j,marker='o',color='black')
				for voisin_comm in aux:
					print(i, j)
					print(voisin_comm)
					plt.plot([pts_x,voisin_comm['x']],[pts_y,voisin_comm['y']], color='red',linewidth=2)
			elif (typ == Type.Cible):
				plt.scatter(i, j,marker='o',color='grey')
				color = "%06x" % np.random.randint(0, 0xFFFFFF)
				for voisin_detect in aux:
					# print(voisin_detect)
					plt.plot([pts_x,voisin_detect['x']],[pts_y,voisin_detect['y']],linewidth=1,color="#"+color)
	plt.show()
