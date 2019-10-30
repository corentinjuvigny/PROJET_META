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

	def get_name(self):
		return self.name

	def set_name(self,name):
		self.name = name

	def get_x(self):
		return self.x

	def set_x(self,x):
		self.x = x

	def get_y(self):
		return self.y

	def set_y(self,y):
		self.y = y

	def get_typ(self):
		return self.typ

	def set_typ(self,typ):
		self.typ = typ

	def get_aux(self):
		return self.aux

	def set_aux(self,aux):
		self.aux = aux


