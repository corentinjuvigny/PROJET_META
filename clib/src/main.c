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
#include "check.h"

int main(int argc, char* argv[])
{
  TPointFile* pf;
  TPointFile* new_pf;

  char* filename = "inst/truncated/captANOR900_15_20.dat";
  double communication_radius = 1.00001;
  double capture_radius = 1.00001;
  int size = 5;

  double phi = 0.999995;
  int step = 2;
  double T_initial = 50.0;
  int nb_iterations = 10;

  double g_time = 10.0;
  int file_mode = 0;
  int graphic = 0;
  nameProcessus = argv[0];

  check_and_set(&filename,&communication_radius,&capture_radius,&size,&phi,&step,&T_initial,&nb_iterations,&g_time,&file_mode,&graphic,argv,argc);

  printf("\n############### PARAMETERS ###############\n");
  printf("filename : %s\n",filename);
  printf("communication_radius : %f\n",communication_radius);
  printf("capture_radius : %f\n",capture_radius);
  printf("size : %d\n",size);
  printf("phi : %f\n",phi);
  printf("step : %d\n",step);
  printf("T_initial : %f\n",T_initial);
  printf("nb_iterations : %d\n",nb_iterations);
  printf("time : %f\n",g_time);
  printf("file_mode : %d\n",file_mode);
  printf("graphic : %d\n",graphic);

  /************************************************************************/
  /************************************************************************/
  /***************************** WITH A FILE ******************************/
  /************************************************************************/
  /************************************************************************/

  if(file_mode){
    pf = read_point_file(filename,communication_radius,capture_radius);
    new_pf = read_point_file(filename,communication_radius,capture_radius);

  }
  else{
    pf = create_point_file(size,communication_radius,capture_radius);
    new_pf = create_point_file(size,communication_radius,capture_radius);
  }

  if (pf == NULL) return 1;
  if (new_pf == NULL) return 1;

  /************************************************************************/
  /************************************************************************/
  /************** GREEDY CONSTRUCTION AND SIMULATED ANNEALING *************/
  /************************************************************************/
  /************************************************************************/

  greedy_construction(pf);
  printf("\n############### RESULT GREEDY CONSTRUCTION ###############\n");
  printf("NUMBER OF TARGETS : %d\n",avl_tree_num_entries(pf->solution)-1);

  if(graphic){
    draw_data(pf,g_time,size);
  }


  BestSolution* result = simulated_annealing(pf, phi, step, T_initial, nb_iterations);
  reconstruct_solution(new_pf,result);
  printf("\n############### RESULT SIMULATED ANNEALING ###############\n");
  printf("NUMBER OF TARGETS : %d\n",avl_tree_num_entries(new_pf->solution));

  if(graphic){
    draw_data(new_pf,g_time,size);
  }

  /************************************************************************/
  /************************************************************************/
  /******************************** DEBUG *********************************/
  /************************************************************************/
  /************************************************************************/

#if DEBUG
  print_pf(new_pf);
#endif


  /************************************************************************/
  /************************************************************************/
  /****************************** CLEANING ********************************/
  /************************************************************************/
  /************************************************************************/

  clean_pf(pf);
  clean_pf(new_pf);
  queue_free(result->best_solution);
  free(result);

  return 0;
}
