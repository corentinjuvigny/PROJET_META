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

  char* filename = argc < 2 ? "../Instances/captANOR1500_21_500.dat" : argv[1];

  nameProcessus = argv[0];

  // TPointFile* pf = read_point_file(filename,communication_radius,capture_radius);

  int size = 5;
  TPointFile* pf = create_point_file(size,communication_radius,capture_radius);

  if (pf == NULL) return 1;

  greedy_construction(pf);
  BestSolution* result = simulated_annealing(pf);

  TPointFile* new_pf = create_point_file(size,communication_radius,capture_radius);
  reconstruct_solution(new_pf,result);

#if DEBUG
  print_pf(new_pf);
#endif

  draw_data(new_pf,1000,size);

  clean_pf(pf);
  clean_pf(new_pf);
  queue_free(result->best_solution);
  free(result);

  return 0;
}
