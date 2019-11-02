/*

Copyright (c) 2019-2020, Adrien BLASSIAU and Corentin JUVIGNY

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/

#include "Python.h"
#include "rwfile.h"
#include "point.h"

void draw_data(TPointFile* pf, float pause, int size)
{
	int i,j,k;
	int nbpoints = pf->nbpoints;
	int taille_grille = size;
	TPoint** points = pf->points;

	Py_Initialize();
	PyRun_SimpleString("import numpy as np");
	PyRun_SimpleString("import matplotlib.pyplot as plt");
	PyRun_SimpleString("import random");

	PyRun_SimpleString("plt.clf()");

	char *pause_string = (char*)malloc(100 * sizeof(char));
	sprintf(pause_string, "plt.pause(%f)",pause);

	char *plt_axis = (char*)malloc(100 * sizeof(char));
	sprintf(plt_axis, "plt.axis([0, %d, 0, %d])",taille_grille,taille_grille);

	char *x_ticks = (char*)malloc(100 * sizeof(char));
	sprintf(x_ticks, "plt.xticks(np.arange(-1, %d, 1))",taille_grille+1);

	char *y_ticks = (char*)malloc(100 * sizeof(char));
	sprintf(y_ticks, "plt.yticks(np.arange(-1, %d, 1))",taille_grille+1);

	PyRun_SimpleString(plt_axis);
	PyRun_SimpleString(x_ticks);
	PyRun_SimpleString(y_ticks);
	PyRun_SimpleString("plt.gca().invert_yaxis()");
	PyRun_SimpleString("plt.grid(True,linewidth=0.25)");

	for (i = 0; i < nbpoints; ++i)
	{
		TPoint* pts = points[i];
		PKind kind = pts->kind;
		AVLTree* aux = pts->aux;
		double pts_x = pts->x;
		double pts_y = pts->y;
		char* pts_name = pts->name;

		AVLTreeValue* aux_list = avl_tree_to_array(aux);
		int aux_length = avl_tree_num_entries(aux);

		if(kind == K_Well){
			char *well_scatter = (char*)malloc(100 * sizeof(char));
			sprintf(well_scatter, "plt.scatter(%f, %f,marker='D',color='black')",pts_x,pts_y);

			char *well_text = (char*)malloc(100 * sizeof(char));
			sprintf(well_text, "plt.text(%f,%f,%s, fontsize=8)",pts_x+0.1,pts_y+0.1,pts_name);

			PyRun_SimpleString(well_scatter);
			PyRun_SimpleString(well_text);
		}
		else if(kind == K_Sensor){
			char *sensor_scatter = (char*)malloc(100 * sizeof(char));
			sprintf(sensor_scatter, "plt.scatter(%f, %f,marker='o',color='black')",pts_x,pts_y);

			char *sensor_text = (char*)malloc(100 * sizeof(char));
			sprintf(sensor_text, "plt.text(%f,%f,%s, fontsize=8)",pts_x+0.1,pts_y+0.1,pts_name);

			PyRun_SimpleString(sensor_scatter);
			PyRun_SimpleString(sensor_text);

			for(j = 0; j < aux_length; j++){
				TPoint* sensor = (TPoint*)(aux_list[j]);
				double sensor_x = sensor->x;
				double sensor_y = sensor->y;

				char *sensor_line = (char*)malloc(100 * sizeof(char));
				sprintf(sensor_line, "plt.plot([%f,%f],[%f,%f], color='red',linewidth=5)",pts_x,sensor_x,pts_y,sensor_y);

				PyRun_SimpleString(sensor_line);
			}
		}
		else{
			char *target_scatter = (char*)malloc(100 * sizeof(char));
			sprintf(target_scatter, "plt.scatter(%f, %f,marker='o',color='grey')",pts_x,pts_y);

			char *target_text = (char*)malloc(100 * sizeof(char));
			sprintf(target_text, "plt.text(%f,%f,%s, fontsize=8)",pts_x+0.1,pts_y+0.1,pts_name);

			PyRun_SimpleString(target_scatter);
			PyRun_SimpleString(target_text);

			for(k = 0; k < aux_length; k++){
				TPoint* target = (TPoint*)(aux_list[k]);
				double target_x = target->x;
				double target_y = target->y;
				PyRun_SimpleString("color = \"#%06x\" % np.random.randint(0, 0xFFFFFF)");

				char *sensor_line = (char*)malloc(100 * sizeof(char));
				sprintf(sensor_line, "plt.plot([%f,%f],[%f,%f], color=color,linewidth=0.5)",pts_x,target_x,pts_y,target_y);

				PyRun_SimpleString(sensor_line);
			}
		}
	}
	PyRun_SimpleString(pause_string);
}