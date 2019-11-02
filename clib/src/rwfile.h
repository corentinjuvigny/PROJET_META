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

/** @file rwfile.h
 *
 * @brief functions to read and create PointFile, the structure that
 * contains all the data we need.
 *
 */

#ifndef __RWFILE__
#define __RWFILE__

#include "include.h"
#include "point.h"

typedef struct _PointFile TPointFile;
typedef const TPointFile CPointFile;

/**
 * \struct _PointFile
 * \brief List of all the points of the problem.
 *
 * Use to store all the data of the problem
 * - points is a list of all the points
 * - kdTree is a tree to find nearest points easily
 * - nbpoints is the nlength of points
 * - solution is the list of all the points where we put a sensor
 * - cover is the number of point covered by at list one sensor. The objective
 * 	 is to cover all the points
 * - communication_radius the communication radius
 * - capture_radius the capture radius
 */
struct _PointFile {
  TPoint**       points;
  struct kdtree* kdTree;
  int            nbpoints;
  AVLTree*       solution;
  int			 cover;
  double 		 communication_radius;
  double	     capture_radius;
};

#define pf_name(i) pf->points[i]->name
#define pf_x(i) pf->points[i]->x
#define pf_y(i) pf->points[i]->y
#define pf_cover() pf->cover
#define pf_communication_radius() pf->communication_radius
#define pf_capture_radius() pf->capture_radius

/**
 * Thi function prints a pf.
 * @param pf The pf we want to print.
 */
void print_pf(TPointFile* pf);

/**
 * This functions cleans a pf.
 * @param pf The pf we want to clean.
 */
void clean_pf(TPointFile* pf);

/**
 * This function is used to read a file containing the data for our problem.
 *
 * @param filename           				The filename.
 * @param communication_radius          	A communication radius.
 * @param capture_radius 					A capture radius.
 * @return									A structure containing all the data
 *                   						of our problem.
 */
TPointFile* read_point_file(char* filename, double communication_radius, double capture_radius);

/**
 * This function is used to create a structure containing the data for our
 * problem.
 *
 * @param size								The height of the grid.
 * @param communication_radius          	A communication radius.
 * @param capture_radius 					A capture radius.
 * @return									A structure containing all the data
 *                   						of our problem.
 */
TPointFile* create_point_file(int size, double communication_radius, double capture_radius);

#endif
