/*

Copyright (c) 2021, Adrien BLASSIAU and Corentin JUVIGNY

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


#include <algorithm>
#include <iostream>
#include <chrono>
#include <iterator>
#include "include.h"
#include "node.hpp"
#include "grid.hpp"
#include "greedy.hpp"
#include "mip.hpp"
#include "rwfile.h"
#include "draw.h"
#include "gridEval.h"
#include "gridNeighbor.h"
#include "gridNeighborhood.h"

int main(int argc, char* argv[])
{
   if ( argc < 2 ) {
      std::cout << "Use a filename as arg" << std::endl;
      return 0;
   }
#if 0
   Node<2> node { Node<2>::K_Well, "node1", {23.5,62.92} };
   std::cout << node << std::endl;
   Grid<2> mygrid { 20,4,2.5,4.2 };
   std::string s { " 2 5.123 7.2" };
   mygrid.insertNode(node_of_line(s,Node<2>::K_Well));
   std::cout << mygrid << std::endl;

#else
   const int win_size = 20;
   const double g_time = 5.0;
   const double communication_radius = 2.00001;
   const double capture_radius = 1.00001;
   const bool draw_result = true;

   std::optional opt_grid = read_node_file_2D(argv[1],communication_radius,capture_radius); 
   if ( opt_grid == std::nullopt ) {
      std::cout << "Error no grid has been generated" << std::endl;
      return 1;
   }

   /* Greedy section */
   auto start = std::chrono::steady_clock::now();
   greedy_construction(*opt_grid);
   auto end = std::chrono::steady_clock::now();
   std::chrono::duration<double> duration = end - start;
   std::cout << "========== Result Greedy Algorithm ==========" << std::endl;
   std::cout << "Number of targets : " << *opt_grid->objective_value() << std::endl;
   std::cout << "Execution time : " << duration.count() << " s" << std::endl;
   //for (auto elem : opt->solution())
   //   std::cout << elem.first << std::endl;
   std::cout << std::endl;
   //mip_resolution(*opt_grid);
   if ( draw_result ) {
      draw_data(*opt_grid,DrawType::Python,g_time,win_size);
      //draw_data(*opt_grid);
   }

   eoGridSolution<2> eog;
   eoGridSolInit(eog,*opt_grid);
   eoGridSolEval eval(*opt_grid);

   eval(eog);
   eog.printOn(std::cout);
   std::cout << std::endl;

   moGridSolNeighborhood<2> gridNeighborhood(*opt_grid);
   moGridSolFullEvalByModif<2> gridFullEval(eval);

   /* =========================================================
    *
    * the cooling schedule of the process
    *
    * ========================================================= */

   // initial temp, factor of decrease, number of steps without decrease, final temp.
   moSimpleCoolingSchedule<eoGridSolution<2>> coolingSchedule(10, 0.995, 100, 0.01);

   moDynSpanCoolingSchedule<eoGridSolution<2>> dynSpanSchedule(40,0.9,200,100,10);


   moTimeContinuator<moGridSolNeighbor<2>> timeContinuator(120);
   moIterContinuator<moGridSolNeighbor<2>> iterContinuator(50000);

   moCombinedContinuator<moGridSolNeighbor<2>> combinedContinuator(timeContinuator);
   combinedContinuator.add(iterContinuator);


   /* =========================================================
    *
    * Comparator of neighbors
    *
    * ========================================================= */

   moSolNeighborComparator<moGridSolNeighbor<2>> solComparator;

   /* =========================================================
    *
    * Example of Checkpointing
    *
    * ========================================================= */

#if 0
   moTrueContinuator<moGridSolNeighbor<2>> continuator;//always continue
   moCheckpoint<moGridSolNeighbor<2>> checkpoint(continuator);
   moFitnessStat<eoGridSolution<2>> fitStat;
   checkpoint.add(fitStat);
   eoFileMonitor monitor("fitness.out", "");
   moCounterMonitorSaver countMon(100, monitor);
   checkpoint.add(countMon);
   monitor.add(fitStat);
#endif

   moSA<moGridSolNeighbor<2>> simulatedAnnealing(gridNeighborhood,eval,gridFullEval,coolingSchedule,solComparator,combinedContinuator);

   moRandomBestHC<moGridSolNeighbor<2>> hillClimber(gridNeighborhood,eval,gridFullEval,combinedContinuator);

   std::cout << "Simulated Annealing" << std::endl;

   simulatedAnnealing(eog);
   //hillClimber(eog);

   std::cout << "Before last eval" << std::endl;
   eog.printOn(std::cout);
   std::cout << std::endl;
   eval(eog);
   std::cout << "After last eval" << std::endl;
   eog.printOn(std::cout);
   std::cout << std::endl;
   std::cout << connectedComponents<2>(eog) << std::endl;
   
#if 0
   std::cout << std::endl;
   for (auto node : opt_grid->solution())
      std::cout << node << std::endl;
   std::cout << opt_grid->cover() << std::endl;
#endif
   if ( draw_result ) {
      draw_data(*opt_grid,DrawType::Python,g_time,win_size);
      //draw_data(*opt_grid);
   }

   opt_grid->end();
#endif

   return 0;
}
