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
#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include "rwfile.h"
#include "avl.h"
#include "queue.h"
#include "point.h"
#include "kdtree.h"


static int* pos_bn(int* pos,const char* file,unsigned long size)
{
    unsigned long i;
    int* pos_bn_tab = xmalloc(size*sizeof(int));
    for (i = 0; i < size; i++) pos_bn_tab[i] = 0;
    for (i = 0, *pos = 0; i < size; i++)
      if (file[i] == '\n')
        pos_bn_tab[(*pos)++] = i;
    return pos_bn_tab;
}

void clean_pf(TPointFile* pf){
  int i;

  for (i = 0; i < pf->nbpoints; i++) {
    xfree(pf_name(i));
    free_node(pf->points[i]);
    pnt_delete(pf->points[i]);
  }
  xfree(pf->points);
  kd_free(pf->kdTree);
  avl_tree_free(pf->solution);
  xfree(pf);
}

void print_pf(TPointFile* pf){
  int i;
  for (i = 0; i < pf->nbpoints; ++i) {
    printf("########## NODE ########\n");
    print_node(pf->points[i]);
    printf("### AUX ###\n");
    print_avl_tree(pf->points[i]->aux,print_node);
    printf("####################\n");
  }
}

static TPoint* point_of_line(char* buf,unsigned long n,PKind kind)
{
  double x, y;
  char* name = xcalloc(100,sizeof(char));
  char cpy[1000] = {[0 ... 999] = 0};

  strncpy(cpy,buf,n);
  sscanf(cpy," %s %lf %lf\n",name,&x,&y);

  return pnt_new(name,x,y,kind,NULL,NULL,NULL);
}

static TPoint* point_of_line_2(double x, double y, int name_int, PKind kind)
{
  char* name = xcalloc(100,sizeof(char));
  sprintf(name, "%d",name_int);
  return pnt_new(name,x,y,kind,NULL,NULL,NULL);
}

static Queue* set_capture_and_communication_queue(TPointFile* pf, TPoint* point, double radius)
{
  struct kdres *covered_node_in_radius_list;
  double pos[2];
  pos[0] = point->x;
  pos[1] = point->y;
  TPoint* node_in_radius;
  void *kdtree = pf->kdTree;

  Queue *queue = queue_new();

  covered_node_in_radius_list = kd_nearest_range(kdtree,pos,radius);

  while(!kd_res_end(covered_node_in_radius_list)){
    node_in_radius = (TPoint*)kd_res_item(covered_node_in_radius_list,pos);

    queue_push_head(queue, node_in_radius);
    kd_res_next(covered_node_in_radius_list);
  }
  kd_res_free(covered_node_in_radius_list);

  return queue;
}

static void finish_pf(TPointFile* pf)
{
  int k;
  double communication_radius = pf->communication_radius;
  double capture_radius = pf->capture_radius;
  int nb_pts = pf->nbpoints;

  for (k = 0; k < nb_pts; k++) {
    TPoint* node = pf->points[k];
    node->capture_queue = set_capture_and_communication_queue(pf,node,capture_radius);
    node->communication_queue = set_capture_and_communication_queue(pf,node,communication_radius);
    node->aux = avl_tree_new((AVLTreeCompareFunc) point_compare);
  }
}

TPointFile* read_point_file(char* filename, double communication_radius, double capture_radius)
{
  struct stat info;
  int* bnp, pos, i;
  double key[2];
  char* buf;
  TPointFile* pf;
  FILE* fd = fopen(filename,"r");

  if (fd == NULL) {
    fprintf(stderr,"%s: error while opening %s in r mode : %s\n",nameProcessus,filename,strerror(errno));
    return NULL;
  }
  if (fstat(fileno(fd),&info) == -1) {
    fprintf(stderr,"%s: error stat : %s\n",nameProcessus,strerror(errno));
    return NULL;
  }
  buf = xmalloc(info.st_size);
  if (fread(buf,sizeof(char),info.st_size,fd) == 0) {
    fprintf(stderr,"%s: error while reading of %s : %s\n",nameProcessus,filename,strerror(errno));
    return NULL;
  }
  fclose(fd);

  bnp = pos_bn(&pos,buf,info.st_size);

  pf = xmalloc(sizeof(*pf));

  pf->nbpoints = pos;
  pf->kdTree = kd_create(2);
  pf->points = xmalloc(pos * sizeof(TPoint));
  pf->solution = avl_tree_new((AVLTreeCompareFunc) point_compare);
  pf->cover = pos - 1;
  pf->communication_radius = communication_radius;
  pf->capture_radius = capture_radius;

  pf->points[0] = point_of_line(buf,bnp[0],K_Well);
  key[0] = pf_x(0);
  key[1] = pf_y(0);
  kd_insert(pf->kdTree,key,pf->points[0]);
  for (i = 0; i < pos-1; i++) {
    pf->points[i+1] = point_of_line(buf+bnp[i]+1,bnp[i+1]-bnp[i],K_Target);
    key[0] = pf_x(i+1);
    key[1] = pf_y(i+1);
    kd_insert(pf->kdTree,key,pf->points[i+1]);
  }

  finish_pf(pf);

  xfree(bnp);
  xfree(buf);

  return pf;
}

TPointFile* create_point_file(int size, double communication_radius, double capture_radius)
{
  int nb_pts = size*size;
  double key[2];
  int k = 0;
  int i,j;
  TPointFile* pf;

  pf = xmalloc(sizeof(*pf));

  pf->nbpoints = nb_pts;
  pf->kdTree = kd_create(2);
  pf->points = xmalloc(nb_pts * sizeof(TPoint));
  pf->solution = avl_tree_new((AVLTreeCompareFunc) point_compare);
  pf->cover = nb_pts - 1;
  pf->communication_radius = communication_radius;
  pf->capture_radius = capture_radius;

  for (i = 0; i < size; ++i){
    for (j = 0; j < size; ++j)
    {
      pf->points[k] = point_of_line_2(i,j,k,K_Target);
      key[0] = pf_x(k);
      key[1] = pf_y(k);
      kd_insert(pf->kdTree,key,pf->points[k]);
      k++;
    }
  }
  pf->points[0]->kind = K_Well;

  finish_pf(pf);

  return pf;
}
