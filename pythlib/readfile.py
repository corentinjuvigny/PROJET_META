import sys
from grille import *
from scipy import spatial
import numpy as np


def read_file(filename):
    content = ""
    point_list = []
    point_list_for_kd_tree = []
    try:
        with open(filename,'r') as f:
            content = [l.replace('\n','') for l in f.readlines()] #on élimine les \n
    except IOError:
        print('Impossible d\'ouvrir le fichier %s' % filename)

    s = [e for e in content[0].split(' ') if e != '']
    point_list.append(Point(int(s[0]),float(s[1]),float(s[2]),Type.Puits,[]))
    point_list_for_kd_tree.append([float(s[1]),float(s[2])])
    for l in content[1:]:
        s = [e for e in l.split(' ') if e != '']
        point_list.append(Point(int(s[0]),float(s[1]),float(s[2]),Type.Cible,[]))
        point_list_for_kd_tree.append([float(s[1]),float(s[2])])
    return (point_list,point_list_for_kd_tree)

# print(read_file("../Instances/captANOR900_15_20.dat"))

# points = read_file("../Instances/InstancesTrunc/captTRUNC1223_40_40.dat")
# point_list_for_kd_tree = points[1]

# kd_tree = spatial.KDTree(point_list_for_kd_tree)
# res = tree.query_ball_point([4, 4], 3)

