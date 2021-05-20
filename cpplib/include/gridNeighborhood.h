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

#include <chrono>
#include <cmath>
#include <iterator>
#include <mo>
#include <utility>
#include <random>
#include "gridEval.h"
#include "gridNeighbor.h"
#include "greedy.hpp"

template <size_t d>
class moGridSolNeighborhood : public moRndNeighborhood<moGridSolNeighbor<d>>
{
   public:
      typedef moGridSolNeighbor<d> Neighbor;
      typedef typename Neighbor::EOT EOT;

      moGridSolNeighborhood(Grid<d> &grid) : _grid(grid), _generator()
      {
         unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
         _generator.seed(seed); 
      }

      virtual bool hasNeighbor(EOT &solution) { return solution.size() > 1; } //There is always an available neighbor

      virtual void init(EOT &solution, Neighbor &current);

      virtual void next(EOT &solution, Neighbor &current);

      virtual bool cont(EOT &solution) { return solution.size() > 1; }

      virtual std::string className() const
      {
         return "moGridSolNeighborhood";
      }
   private:
      Grid<d> &_grid;
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
      if ( count > 100000 ) {
         return NULL;
      }
   } while ( node->kind() != Node<d>::K_Target );

   return node;
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
      std::uniform_int_distribution<int> dist(1);
      auto sol_it = grid.solution().cbegin();
      std::advance(sol_it,dist(gen)%grid.solution().size());
      node = *sol_it;
      count++;
      if ( count > 100000 ) {
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
void moGridSolNeighborhood<d>::init(EOT &solution, Neighbor &current)
{
   if ( _grid.solution().size() < 1 ) {
      greedy_construction(_grid);
      eoGridSolInit(solution,_grid);
   }
   GridNeighbor neighbor = foundNextNeighbor<d>(_grid,_generator);
   current.initNeighbor(std::move(neighbor),&_grid);
}

template <size_t d>
void moGridSolNeighborhood<d>::next(EOT &, Neighbor &current)
{
   current.setNeighbor(foundNextNeighbor<d>(_grid,_generator));
}

#endif // __GRIDNEIGHBORHOOD_H__