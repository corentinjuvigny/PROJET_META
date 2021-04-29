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

/** @file point.h
 *
 * @brief functions to manage a point
 *
 */

#ifndef __POINT__
#define __POINT__

#include "tools.h"
#include "avl.h"
#include "queue.h"
#define Inline static inline


typedef enum _PKind PKind;
enum _PKind {
  K_Well   = 0,   // Kind of the original point
  K_Target = 1,   // Kind of target points
  K_Sensor = 2    // Kind of sensor points
};

/**
 * \struct _TPoint
 * \brief A 2D point of our problem.
 *
 * Use to store all the data related to a point
 * - name is the name of the point.
 * - x and y are the coordinates of the points.
 * - kind is the kind of point (sensor or target or well).
 * - capture queue are all the points in the capture radius of the point.
 * - communication_queue are all the points in the comunication queue of the
 *   point.
 * - aux : if the point is a target, it contains all the sensors in its capture
 *   radius. Whereas, it contains all the sensors and the well in the
 *   communication radius of the point.
 */
typedef struct _TPoint TPoint;
typedef const TPoint CPoint;
struct _TPoint {
  char*   name;
  double  x;
  double  y;
  PKind   kind;
  Queue*  capture_queue;
  Queue* 	communication_queue;
  AVLTree*   aux;
};

/**
 * This function is used to print a point.
 *
 * @param node  The point we want to print.
 */
extern void print_node(void* node);

/**
 * This function is used to create a point.
 *
 * @param  name                The name of the point.
 * @param  x                   The x coordinate of the point.
 * @param  y                   The y coordinate of the point.
 * @param  kd                  The kind of the point.
 * @param  capture_queue       The capture queue of the point.
 * @param  communication_queue The communication queue of the point.
 * @param  aux                 The aux data of the point (see below).
 *
 * @return                     A new point.
 */
extern TPoint* pnt_new(char* name,double x,double y,PKind kd,
                       Queue* capture_queue,Queue* communication_queue,AVLTree* aux);

/**
 * This function is used to compare two points by using their names.
 *
 * @param  point1 A point.
 * @param  point2 A point.
 *
 * @return        An integer given by strcmp applied on the name of the points.
 */
extern int point_compare(void *point1, void *point2);

/**
 * This function is used to free a node.
 *
 * @param node The node we want to free.
 */
extern void free_node(void* node);


Inline TPoint* pnt_new_clone(CPoint* src)
{ return pnt_new(src->name,src->x,src->y,src->kind,NULL,NULL,NULL); }

Inline void pnt_delete(TPoint* pnt) { xfree(pnt); }

#endif
