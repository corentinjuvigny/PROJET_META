#include "include.h"
#include "rwfile.h"
#include "point.h"
#include "dist.h"
#include "greedy.h"
#include "avl.h"
#include "queue.h"
#include "draw.h"
#include "annealing.h"

int main(int argc, char* argv[])
{
  double communication_radius = 1.00001;
  double capture_radius = 2.00001;
  int size = 10;
  int debug = 0;

  int i;
  char* filename = argc < 2 ? "../Instances/captANOR1500_21_500.dat" : argv[1];

  // TPointFile* pf = read_point_file(filename,communication_radius,capture_radius);
  TPointFile* pf = create_point_file(size,communication_radius,capture_radius);

  if (pf == NULL) return 1;

  greedy_construction(pf);
  simulated_annealing(pf);
  printf("FIN\n");
  if (debug){
    printf("AUX\n");
    int e;
    for (e = 0; e < pf->nbpoints; ++e)
    {
      printf("########## NODE ########\n");
      print_node(pf->points[e]);
      printf("### AUX ###\n");
      print_avl_tree(pf->points[e]->aux,print_node);
      printf("####################\n");
    }
    printf("AUX\n");
  }

  if (debug){
    printf("RESULT : %d\n",avl_tree_num_entries(pf->solution));
    printf("###### FIN GREED ########\n");
  }

  for (i = 0; i < pf->nbpoints; i++) {
    xfree(pf_name(i));
    free_node(pf->points[i]);
    pnt_delete(pf->points[i]);
  }
  xfree(pf->points);
  kd_free(pf->kdTree);
  avl_tree_free(pf->solution);
  xfree(pf);

  return 0;
}
