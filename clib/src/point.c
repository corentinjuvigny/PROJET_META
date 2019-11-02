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

#include "point.h"
#include "avl.h"
#include "queue.h"

extern TPoint* pnt_new(char* name,double x,double y,PKind kd,Queue* capture_queue,Queue* communication_queue,AVLTree* aux)
{
  TPoint* pnt = xmalloc(sizeof(*pnt));
  pnt->name = name;
  pnt->x = x;
  pnt->y = y;
  pnt->kind = kd;
  pnt->capture_queue = capture_queue;
  pnt->communication_queue = communication_queue;
  pnt->aux = aux;
  return pnt;
}


extern void free_node(void* node){
	TPoint* point = (TPoint*)node;
	queue_free(point->capture_queue);
  	queue_free(point->communication_queue);
  	avl_tree_free(point->aux);
}

extern void print_node(void* node){
	TPoint* point = (TPoint*)node;
	printf("%s (%f,%f) de type %d \n",point->name,point->x,point->y,point->kind);
}

extern int point_compare(void *point1, void *point2)
{
	int result;
	TPoint *point1_to_compare = (TPoint*)point1;
	TPoint *point2_to_compare = (TPoint*)point2;

	char *point1_name = point1_to_compare->name;
	char *point2_name = point2_to_compare->name;

	result = strcmp(point1_name,point2_name);

	if (result < 0) {
		return -1;
	} else if (result > 0) {
		return 1;
	} else {
		return 0;
	}
}



