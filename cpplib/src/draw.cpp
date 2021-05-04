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

#include <cstdlib>
#include "Python.h"
#include "node.hpp"
#include "grid.hpp"

void draw_data_2D_Python(const Grid<2> &g, const float pause, const int size)
{
	const int taille_grille = size;
   const std::vector<Grid<2>::SNode> nodes = g.nodes();

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

   for (const auto &node : nodes) {
		const Node<2>::Kind &kind = node->kind();
		const Grid<2>::AVLNodes &aux = node->aux();
      const auto [ node_x, node_y ] = node->coord();
		const char* node_name = node->name().c_str();

      switch (kind) {
         [[unlikely]]
         case Node<2>::K_Well:
         {
		      char *well_scatter = (char*)malloc(100 * sizeof(char));
		      sprintf(well_scatter, "plt.scatter(%f, %f,marker='D',color='black')",node_x,node_y);

		      char *well_text = (char*)malloc(100 * sizeof(char));
		      sprintf(well_text, "plt.text(%f,%f,%s, fontsize=8)",node_x+0.1,node_y+0.1,node_name);

		      PyRun_SimpleString(well_scatter);
		      PyRun_SimpleString(well_text);
            break;
		   }
         [[likely]]
         case Node<2>::K_Sensor:
         {
		      char *sensor_scatter = (char*)malloc(100 * sizeof(char));
		      sprintf(sensor_scatter, "plt.scatter(%f, %f,marker='o',color='black')",node_x,node_y);

		      char *sensor_text = (char*)malloc(100 * sizeof(char));
		      sprintf(sensor_text, "plt.text(%f,%f,%s, fontsize=8)",node_x+0.1,node_y+0.1,node_name);

		      PyRun_SimpleString(sensor_scatter);
		      PyRun_SimpleString(sensor_text);

            for (const auto &sensor_elem : aux) {
		         const Node<2>* sensor = sensor_elem;
               const auto [ sensor_x, sensor_y ] = sensor->coord();

		         char *sensor_line = (char*)malloc(100 * sizeof(char));
		         sprintf(sensor_line, "plt.plot([%f,%f],[%f,%f], color='red',linewidth=3)",node_x,sensor_x,node_y,sensor_y);

		         PyRun_SimpleString(sensor_line);
            }
		      break;
         }
         [[likely]]
         case Node<2>::K_Target:
         {
		      char *target_scatter = (char*)malloc(100 * sizeof(char));
		      sprintf(target_scatter, "plt.scatter(%f, %f,marker='o',color='grey')",node_x,node_y);

		      char *target_text = (char*)malloc(100 * sizeof(char));
		      sprintf(target_text, "plt.text(%f,%f,%s, fontsize=8)",node_x+0.1,node_y+0.1,node_name);

		      PyRun_SimpleString(target_scatter);
		      PyRun_SimpleString(target_text);

            for (const auto &target_elem : aux) {
               const Node<2>* target = target_elem;
               const auto [ target_x, target_y ] = target->coord();
		         PyRun_SimpleString("color = \"#%06x\" % np.random.randint(0, 0xFFFFFF)");

		         char *target_line = (char*)malloc(100 * sizeof(char));
		         sprintf(target_line, "plt.plot([%f,%f],[%f,%f], color=color,linewidth=0.5)",node_x,target_x,node_y,target_y);

		         PyRun_SimpleString(target_line);
            }
		   }
	   }
   }
	PyRun_SimpleString(pause_string);
}
