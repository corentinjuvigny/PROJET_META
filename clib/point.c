#include "point.h"

extern TPoint* pnt_new(char* name,double x,double y,PKind kd,void* aux)
{
  TPoint* pnt = xmalloc(sizeof(*pnt));
  pnt->name = name;
  pnt->x = x;
  pnt->y = y;
  pnt->kind = kd;
  pnt->aux = aux;
  return pnt;
}
