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

/**
 * @file gridNeighborhood.h
 *
 * @brief neighborhood class for paradiseo algorithms
 *
 *
 */

#ifndef __GRIDNEIGHBORHOOD_H__
#define __GRIDNEIGHBORHOOD_H__

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iterator>
#include <mo>
#include <pstl/glue_execution_defs.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <utility>
#include <random>
#include <mutex>
#include "gridEval.h"
#include "gridNeighbor.h"
#include "greedy.hpp"

/* ========================== Random neighborhood ========================== */
template <size_t d>
class moGridSolRndNeighborhood : public moRndNeighborhood<moGridSolNeighbor<d>>
{
   public:
      typedef moGridSolNeighbor<d> Neighbor;
      typedef typename Neighbor::EOT EOT;

      moGridSolRndNeighborhood(Grid<d> &grid) : _grid(grid), _generator()
      {
         unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
         _generator.seed(seed); 
      }

      virtual bool hasNeighbor(EOT &solution) { return solution.size() > 1; }

      virtual void init(EOT &solution, Neighbor &current);

      virtual void next(EOT &solution, Neighbor &current);

      virtual bool cont(EOT &solution) { return solution.size() > 1; }

      virtual std::string className() const
      {
         return "moGridSolRndNeighborhood";
      }
   private:
      // The data of the problem
      Grid<d> &_grid;
      // A random generator
      std::default_random_engine _generator;
};

template <size_t d>
Node<d>* nodeInInsertNeighbor( const Grid<d> &grid
                             , std::default_random_engine &gen )
{
   Node<d>* node;
   int count = 0;
   do {
      std::uniform_int_distribution<int> dist;
      auto sol_it = grid.solution().cbegin();
      std::advance(sol_it,dist(gen)%grid.solution().size());
      const Node<d>* sensor_selected = *sol_it;
      auto it = (*sol_it)->capture_queue().begin();
      std::advance(it,dist(gen)%(*sol_it)->capture_queue().size());
      node = *it;
      count++;
      if ( count > 1000 ) {
         return NULL;
      }
   } while ( node->kind() != Node<d>::K_Target );

   return node;
}

template <size_t d>
std::vector<Node<d>*> removableSensors(const Grid<d> &grid)
{
   std::vector<Node<d>*> rs;
   std::mutex mutex;
   std::for_each( std::execution::par
                , grid.solution().cbegin()
                , grid.solution().cend()
                , [&](Node<d> * const sensor) {
   //for (auto sensor : grid.solution()) {
      bool is_removable = true;
      bool is_covered = false;
      // Can't remove the well
      if ( sensor->kind() == Node<d>::K_Well )
         return;
         //continue;
      // Checks if all of the target in the capture radius of the sensor
      // are also covered by another sensor
      for (auto node : sensor->capture_queue()) {
         // Checks if node != sensor
         if ( equal_coord<d>(sensor->coord(),node->coord()) )
               continue;
         if ( node->kind() == Node<d>::K_Target && node->aux().size() <= 1)
            is_removable &= false;
         // Checks if itself covered
         else if ( node->kind() == Node<d>::K_Sensor )
            is_covered = true;
      }
      // Checks if the sensors' graph without the sensor is still connected;
      if ( is_removable && is_covered )
         is_removable &= connectedComponents(grid,sensor) == 1;
      if ( is_removable && is_covered ) {
         std::lock_guard<std::mutex> lock(mutex);
         rs.push_back(sensor);
      }
   } );
   return rs;
}

template <size_t d>
Node<d>* nodeInRemoveNeighbor( const Grid<d> &grid
                             , std::default_random_engine &gen )
{
   if ( grid.solution().size() <= 1 )
      return NULL;
   Node<d>* node;
   int count = 0;
   do {
      std::uniform_int_distribution<int> dist;
      auto removable_sensors = removableSensors(grid);
      if ( removable_sensors.empty() )
         return NULL;
      node = removable_sensors[dist(gen)%removable_sensors.size()];
      count++;
      if ( count > 1000 ) {
         return NULL;
      }
   } while ( node->kind() != Node<d>::K_Sensor );
   return node;
}

template <size_t d>
GridNeighbor<d> foundNextNeighbor(const Grid<d> &grid, std::default_random_engine &generator)
{
   GridNeighbor<d> neighbor;
   std::uniform_int_distribution<int> distribution(0,1);
   if ( distribution(generator) == 0 ) {
      // Add case
      neighbor = InsertGridNeighbor<d>(nodeInInsertNeighbor(grid,generator));
      if ( neighbor.node == NULL )
         neighbor = RemoveGridNeighbor<d>(nodeInRemoveNeighbor(grid,generator));
   } else {
      // Remove case
      neighbor = RemoveGridNeighbor<d>(nodeInRemoveNeighbor(grid,generator));
      if ( neighbor.node == NULL )
         neighbor = InsertGridNeighbor<d>(nodeInInsertNeighbor(grid,generator));
   }
   return neighbor;
}

template <size_t d>
void moGridSolRndNeighborhood<d>::init(EOT &solution, Neighbor &current)
{
   if ( _grid.solution().size() < 1 ) {
      greedy_construction(_grid);
      eoGridSolInit(solution,_grid);
   }
   GridNeighbor neighbor = foundNextNeighbor<d>(_grid,_generator);
   current.initNeighbor(std::move(neighbor),&_grid);
}

template <size_t d>
void moGridSolRndNeighborhood<d>::next(EOT &, Neighbor &current)
{
   current.setNeighbor(foundNextNeighbor<d>(_grid,_generator));
}

/* ========================== Order neighborhood ========================== */

template <size_t d>
std::vector<GridNeighbor<d>> availableNeighbors(const Grid<d> &grid)
{
   std::vector<GridNeighbor<d>> neighbors;

   // First we look for all of the insertable neighbors
   for (auto sensor : grid.solution()) {
      for (auto target : sensor->capture_queue()) {
         if ( target->kind() == Node<d>::K_Target )
            neighbors.push_back(InsertGridNeighbor(target));
      }
   }
   // Then we look for all of the removable neighbors
   std::vector<Node<d>*> removables = removableSensors(grid);
   for (auto sensor : removables)
      neighbors.push_back(RemoveGridNeighbor(sensor));

   return neighbors;
}

template <size_t d>
class moGridSolOrderNeighborhood : public moNeighborhood<moGridSolNeighbor<d>>
{
   public:
      typedef moGridSolNeighbor<d> Neighbor;
      typedef typename Neighbor::EOT EOT;

      moGridSolOrderNeighborhood(Grid<d> &grid) : _grid(grid), _neighborList(), _pos(0) { }

      virtual bool hasNeighbor(EOT &solution) { return solution.size() > 1; }

      virtual void init(EOT &solution, Neighbor &current)
      {
         if ( _grid.solution().size() < 1 ) {
            greedy_construction(_grid);
            eoGridSolInit(solution,_grid);
         }
         _neighborList = availableNeighbors(_grid);
         _pos = 0;
         
         current.initNeighbor(std::move(_neighborList[_pos++]),&_grid);
      }

      virtual void next(EOT&, Neighbor &current)
      {
         current.setNeighbor(std::move(_neighborList[_pos++]));
      }

      virtual bool cont(EOT&) { return _pos < _neighborList.size(); }

      virtual std::string className() const
      {
         return "moGridSolOrderNeighborhood";
      }
   private:
      // The data of the problem
      Grid<d> &_grid;
      // A vector containing all of the possible neighbors of a solution
      std::vector<GridNeighbor<d>> _neighborList;
      // The current neighbor in _neighborList;
      size_t _pos;
};


#endif // __GRIDNEIGHBORHOOD_H__
