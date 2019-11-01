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
  char* c = "test";
  char* filename = argc < 2 ? "../Instances/captANOR225_9_20.dat" : argv[1];
  void *tree;
  struct kdres *presults;

  TPoint* pch;
  double pos[2], dist;
  double pt[2] = {1, 0};
  double radius = 1.001;

  TPoint* pnt = pnt_new(c,0.2598,0.1478,K_Target,NULL,NULL,NULL);
  printf("%s %lf %lf %d %p\n",pnt->name,pnt->x,pnt->y,pnt->kind,pnt->aux);
  TPoint* pntbis = pnt_new_clone(pnt);
  printf("%s %lf %lf %d %p\n",pntbis->name,pntbis->x,pntbis->y,pntbis->kind,pntbis->aux);
  pnt_delete(pntbis);
  pnt_delete(pnt);

  TPointFile* pf = read_point_file(filename,communication_radius,capture_radius);
  // TPointFile* pf = create_point_file(size,communication_radius,capture_radius);
  if (pf == NULL) return 1;

  AVLTree *avl_tree = avl_tree_new((AVLTreeCompareFunc) point_compare);

  printf("TEST DE LA TAILLE !!!\n");
  printf("Nombre d'entree %d\n",avl_tree_num_entries(avl_tree));
  printf("TEST DE LA TAILLE !!!\n");

  for (i = 0; i < pf->nbpoints; i++){
    printf("%s %lf %lf\n",pf_name(i),pf_x(i),pf_y(i));
    avl_tree_insert(avl_tree,&(pf->points[i]->name), pf->points[i]);
  }

  printf("##########\n");
  print_avl_tree(avl_tree, print_node);
  printf("##########\n");

  avl_tree_change_value(avl_tree,&(pf->points[0]->name), pf->points[1]);

  char * test_pt = pf->points[0]->name;

  printf("Nombre d'entree %d\n",avl_tree_num_entries(avl_tree));

  TPoint * test_res = avl_tree_lookup(avl_tree,&test_pt);
  printf("ok\n");
  if (test_res!=NULL){
     printf("Nombre d'entree %f\n",test_res->y);
  }
  else{
    printf("C'est nil\n");
  }

  Queue* capture_queue = pf->points[0]->capture_queue;
  Queue* communication_queue = pf->points[0]->communication_queue;

  printf("On print la queue\n");
  print_queue(capture_queue, print_node);
  printf("On print la queue\n");
  print_queue(communication_queue, print_node);
  printf("On print la queue\n");

  greedy_construction(pf);

  draw_data(pf,30,size);

  tree = pf->kdTree;
  presults = kd_nearest_range(tree,pt,radius);

  /* print out all the points found in results */
  printf( "found %d results:\n", kd_res_size(presults) );

  while( !kd_res_end( presults ) ) {
    /* get the data and position of the current result item */
    pch = (TPoint*)kd_res_item( presults, pos );

    /* compute the distance of the current result from the pt */
    dist = sqrt( dist_sq( pt, pos, 2 ) );
    printf("distance : %f",dist);
    /* print out the retrieved data */
    print_node(pch);

    /* go to the next entry */
    kd_res_next( presults );
  }

  /* print out all the points found in results */
  printf( "found %d results:\n", kd_res_size(presults) );

  for (i = 0; i < pf->nbpoints; i++) {
    xfree(pf_name(i));
    pnt_delete(pf->points[i]);
  }
  xfree(pf->points);
  kd_free(pf->kdTree);
  xfree(pf);

  return 0;
}
