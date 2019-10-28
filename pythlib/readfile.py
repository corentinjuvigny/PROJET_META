import sys
from grille import *

def read_file(filename):
    content = ""
    point_list = []
    try:
        with open(filename,'r') as f:
            content = [l.replace('\n','') for l in f.readlines()] #on élimine les \n
    except IOError:
        print('Impossible d\'ouvrir le fichier %s' % filename)
    
    s = [e for e in content[0].split(' ') if e != '']
    point_list.append(Point(s[0],float(s[1]),float(s[2]),Type.Puits,[]))
    for l in content[1:]:
        s = [e for e in l.split(' ') if e != '']
        point_list.append(Point(s[0],float(s[1]),float(s[2]),Type.Capteur,[]))
    return point_list

print(read_file("../Instances/captANOR900_15_20.dat"))