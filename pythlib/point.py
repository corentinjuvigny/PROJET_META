from enum import Enum

class Type(Enum):
	Puits = 1
	Cible = 2
	Capteur = 3

class Point:
	def __init__(self,name,x,y,typ,aux):
		self.name = name
		self.x = x
		self.y = y
		self.typ = typ
		self.aux = aux

	def __repr__(self):
		return "name = {}, x = {}, y = {}, type = {}, aux = {}".format(self.name,self.x,self.y,self.typ,self.aux)
