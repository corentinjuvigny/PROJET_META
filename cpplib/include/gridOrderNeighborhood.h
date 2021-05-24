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
 * @file gridOrderNeighborhood.h
 *
 * @brief ordered neighborhood class for paradiseo algorithms
 *
 *
 */

#ifndef __GRIDORDERNEIGHBORHOOD_H__
#define __GRIDORDERNEIGHBORHOOD_H__

#include <mo>
#include "gridNeighbor.h"

template <size_t d>
class moGridSolOrderNeighborhood : public moNeighborhood<moGridSolNeighbor<d>>
{
   public:
      typedef moGridSolNeighbor<d> Neighbor;
      typedef typename Neighbor::EOT EOT;

      moGridSolOrderNeighborhood(Grid<d> &grid) : _grid(grid){ }

      virtual bool hasNeighbor(EOT &solution) { return solution.size() > 1; }

      virtual void init(EOT &solution, Neighbor &current);

      virtual void next(EOT &solution, Neighbor &current);

      virtual bool cont(EOT &solution) { return solution.size() > 1; }

      virtual std::string className() const
      {
         return "moGridSolNeighborhood";
      }
   private:
      Grid<d> &_grid;
      std::vector<GridNeighbor<d>> _neighborList;
};


#endif //__GRIDORDERNEIGHBORHOOD_H__
