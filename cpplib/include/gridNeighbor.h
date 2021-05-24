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
 * @file gridNeighbor.h
 *
 * @brief neighbor class for paradiseo algorithms
 *
 *
 */

#ifndef __GRIDNEIGHBOR_H__
#define __GRIDNEIGHBOR_H__

#include <mo>
#include "gridEval.h"

template <size_t d>
struct GridNeighbor
{
   enum Kind { INSERT, REMOVE };
   Kind kind;
   Node<d>* node;

   friend inline bool operator==(const GridNeighbor &lhs, const GridNeighbor &rhs)
   {
      if ( lhs.node == NULL )
         return rhs.node == NULL ? true : false;
      if ( rhs.node == NULL )
         return lhs.node == NULL ? true : false;
      return (lhs.kind == rhs.kind) && (equal_coord<d>(lhs.node->coord(),rhs.node->coord()));
   }
};

template <size_t d>
struct InsertGridNeighbor : public GridNeighbor<d>
{
   InsertGridNeighbor(Node<d>* node = NULL)
   {
      this->kind = GridNeighbor<d>::INSERT;
      this->node = node;
   }
};

template <size_t d>
struct RemoveGridNeighbor : public GridNeighbor<d>
{
   RemoveGridNeighbor(Node<d>* node = NULL)
   {
      this->kind = GridNeighbor<d>::REMOVE;
      this->node = node;
   }
};

template <size_t d>
class moGridSolNeighbor : public moBackableNeighbor<eoGridSolution<d>>
{
   public:
      moGridSolNeighbor() : moBackableNeighbor<eoGridSolution<d>>(), _neighbor(), _grid(nullptr) { }

      void move(eoGridSolution<d>& solution)
      {
         switch ( _neighbor.kind ) {
            case GridNeighbor<d>::INSERT:
               solution.push_back(_neighbor.node);
               _grid->add_sensor_to_solution(_neighbor.node);
               break;
            case GridNeighbor<d>::REMOVE:
               solution.erase(std::remove(solution.begin(),solution.end(),_neighbor.node));
               _grid->remove_sensor_to_solution(_neighbor.node);
               break;
            default:
               break;
         }
         solution.invalidate();
      }

      void moveBack(eoGridSolution<d>& solution)
      {
         switch ( _neighbor.kind ) {
            case GridNeighbor<d>::INSERT:
               _grid->remove_sensor_to_solution(_neighbor.node);
               solution.erase(std::remove(solution.begin(),solution.end(),_neighbor.node));
               break;
            case GridNeighbor<d>::REMOVE:
               _grid->add_sensor_to_solution(_neighbor.node);
               solution.push_back(_neighbor.node);
               break;
            default:
               break;
         }
         solution.invalidate();
      }

      const GridNeighbor<d> &neighbor() const { return _neighbor; }

      void initNeighbor(GridNeighbor<d>&& neighbor, Grid<d>* grid)
      {
         _neighbor = neighbor;
         _grid = grid;
      }
      void setNeighbor(GridNeighbor<d> &&neighbor)
      {
         _neighbor = neighbor;
      }

      bool equals(moGridSolNeighbor& neighbor)
      {
         return neighbor.neighbor() == _neighbor;
      }

      std::string className() const
      {
         return "moGridSolNeighbor";
      }

   private:
      Grid<d>* _grid;
      GridNeighbor<d> _neighbor;
};

template <size_t d>
using moGridSolFullEvalByModif = moFullEvalByModif<moGridSolNeighbor<d>>;

#endif // __GRIDNEIGHBOR_H__
