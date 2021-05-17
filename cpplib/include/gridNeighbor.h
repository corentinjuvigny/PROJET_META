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

#include <map>
#include <mo>
#include "gridEval.h"

template <size_t d>
class moGridSolNeighbor : public moBackableNeighbor<eoGridSolution<d>>
{
   public:
      typedef std::map<eoGridSolution<d>,eoGridSolution<d>> MapSolution;
      
      moGridSolNeighbor() : moBackableNeighbor<eoGridSolution<d>>(), _currentSolution(), _solutions() { }

      void move(eoGridSolution<d>& solution)
      {
         _solutions[_currentSolution] = solution;
         _currentSolution = solution;
         solution.invalidate();
      };
      
      void moveBack(eoGridSolution<d>& solution)
      {
         _currentSolution = _solutions[solution];
         _solutions.erase(solution);
      }

      const eoGridSolution<d> &currentSolution() const { return _currentSolution; }
      const MapSolution &solutions() const { return solutions; }

      bool equals(moGridSolNeighbor& neighbor)
      {
         return neighbor.currentSolution() == this->_currentSolution && neighbor.solutions() == _solutions;
      }

      std::string className() const
      {
         return "moGridSolNeighbor";
      }
   private:
      eoGridSolution<d> _currentSolution;
      MapSolution _solutions;
};

#endif // __GRIDNEIGHBOR_H__
