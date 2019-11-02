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

#include "include.h"
#include "kdtree.h"
#include "rwfile.h"
#include "point.h"
#include "dist.h"
#include "greedy.h"
#include "avl.h"
#include "draw.h"
#include "annealing.h"

int main(int argc, char* argv[])
{
  double communication_radius = 8.00001;
  double capture_radius = 2.00001;
  int size = 5;
  int debug = 0;

  int i;
  char* filename = argc < 2 ? "../Instances/captANOR1500_21_500.dat" : argv[1];

  // TPointFile* pf = read_point_file(filename,communication_radius,capture_radius);
  TPointFile* pf = create_point_file(size,communication_radius,capture_radius);

  if (pf == NULL) return 1;

  greedy_construction(pf);
  BestSolution* result = simulated_annealing(pf);

  print_queue(result->best_solution,print_string);
  printf("SIZE : %d\n",result->size);

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
  queue_free(result->best_solution);
  free(result);
  return 0;
}
