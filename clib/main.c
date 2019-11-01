#include "include.h"
#include "rwfile.h"
#include "point.h"
#include "dist.h"
#include "greedy.h"
#include "avl.h"
#include "queue.h"
#include "draw.h"

int main(int argc, char* argv[])
{
  double communication_radius = 2.00001;
  double capture_radius = 2.00001;
  int size = 10;

  int i;
  char* filename = argc < 2 ? "../Instances/captANOR225_9_20.dat" : argv[1];

  TPointFile* pf = read_point_file(filename,communication_radius,capture_radius);
  // TPointFile* pf = create_point_file(size,communication_radius,capture_radius);

  if (pf == NULL) return 1;

  greedy_construction(pf);

  draw_data(pf,30,size);


  for (i = 0; i < pf->nbpoints; i++) {
    xfree(pf_name(i));
    pnt_delete(pf->points[i]);
  }
  xfree(pf->points);
  kd_free(pf->kdTree);
  xfree(pf);

  return 0;
}
