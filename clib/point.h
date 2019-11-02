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

typedef struct _TPoint TPoint;
typedef const TPoint CPoint;
struct _TPoint {
  char*   name;   				// name of the point
  double  x;      				// x coordonate
  double  y;      				// y coordonate
  PKind   kind;  	 			// point kind
  Queue* capture_queue;			// all the node in his capture radius
  Queue* 	communication_queue;	// all the node in his communication radius
  AVLTree*   aux;				// if it's a target, all the sensor in is
  								// capture radius. if it's a sensor, all the
  								// sensor in
};

extern void print_node(void* node);

extern TPoint* pnt_new(char* name,double x,double y,PKind kd,Queue* capture_queue,Queue* communication_queue,AVLTree* aux);
extern int point_compare(void *point1, void *point2);

extern void free_node(void* node);

Inline TPoint* pnt_new_clone(CPoint* src)
{ return pnt_new(src->name,src->x,src->y,src->kind,NULL,NULL,NULL); }

Inline void pnt_delete(TPoint* pnt) { xfree(pnt); }

#endif
