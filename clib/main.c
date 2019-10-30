#include <stdio.h>
#include "point.h"

int main()
{
  char* c = "test";
  TPoint* pnt = pnt_new(c,0.2598,0.1478,K_Target,NULL);
  printf("%s %lf %lf %d %p\n",pnt->name,pnt->x,pnt->y,pnt->kind,pnt->aux);
  TPoint* pntbis = pnt_new_clone(pnt);
  printf("%s %lf %lf %d %p\n",pntbis->name,pntbis->x,pntbis->y,pntbis->kind,pntbis->aux);
  pnt_delete(pntbis);
  pnt_delete(pnt);
  return 0;
}
