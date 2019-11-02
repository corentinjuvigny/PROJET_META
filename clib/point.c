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



