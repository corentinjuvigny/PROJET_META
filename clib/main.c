#include "include.h"
#include "rwfile.h"
#include "point.h"

int main(int argc, char* argv[])
{
  int i;
  char* c = "test";
  char* filename = argc < 2 ? "../Instances/captANOR225_9_20.dat" : argv[1];
  TPoint* pnt = pnt_new(c,0.2598,0.1478,K_Target,NULL);
  printf("%s %lf %lf %d %p\n",pnt->name,pnt->x,pnt->y,pnt->kind,pnt->aux);
  TPoint* pntbis = pnt_new_clone(pnt);
  printf("%s %lf %lf %d %p\n",pntbis->name,pntbis->x,pntbis->y,pntbis->kind,pntbis->aux);
  pnt_delete(pntbis);
  pnt_delete(pnt);

  TPointFile* pf = read_point_file(filename);

  for (i = 0; i < pf->nbpoints; i++)
    printf("%s %lf %lf\n",pf_name(i),pf_x(i),pf_y(i));

  for (i = 0; i < pf->nbpoints; i++) {
    xfree(pf_name(i));
    pnt_delete(pf->points[i]);
  }
  xfree(pf->points);
  kd_free(pf->kdTree);
  xfree(pf);

  return 0;
}
