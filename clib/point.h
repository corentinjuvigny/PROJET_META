#ifndef __POINT__
#define __POINT__

#include "tools.h"
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
  char*   name;   // name of the point
  double  x;      // x coordonate
  double  y;      // y coordonate
  PKind   kind;   // point kind
  void*   aux;    // 
};

extern TPoint* pnt_new(char* name,double x,double y,PKind kd,void* aux);

Inline TPoint* pnt_new_clone(CPoint* src)
{ return pnt_new(src->name,src->x,src->y,src->kind,src->aux); }

Inline void pnt_delete(TPoint* pnt) { xfree(pnt); }

#endif
