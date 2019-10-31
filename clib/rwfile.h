#ifndef __RWFILE__
#define __RWFILE__

#include "include.h"
#include "point.h"

typedef struct _PointFile TPointFile;
typedef const TPointFile CPointFile;
struct _PointFile {
  TPoint**       points;
  struct kdtree* kdTree;
  int            nbpoints;
};

#define pf_name(i) pf->points[i]->name
#define pf_x(i) pf->points[i]->x
#define pf_y(i) pf->points[i]->y

TPointFile* read_point_file(char* filename);

#endif
