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
 * @file gridEval.h
 *
 * @brief eval functions for paradiseo algorithms
 *
 *
 */

#ifndef __GRIDEVAL_H__
#define __GRIDEVAL_H__

#include <algorithm>
#include <eo>
#include <numeric>
#include "grid.hpp"

template <size_t d>
using eoGridSolution = eoVector<eoMinimizingFitness,Node<d>*>;

template <size_t d = 2>
void eoGridSolInit(eoGridSolution<d> &eog, const Grid<d> &g)
{
   std::for_each( g.solution().cbegin()
                , g.solution().cend()
                , [&](Node<d>* const &n) {
                     eog.push_back(n);
                  } );
}

template <size_t d = 2>
class eoGridSolEval : public eoEvalFunc<eoGridSolution<d>>
{
   public:
      eoGridSolEval() = default;
      eoGridSolEval(Grid<d> &g) : _grid(g) { }
      ~eoGridSolEval() = default;
      void operator()(eoGridSolution<d> &eog)
      {
         auto gridFold = [](double &ebx, const Node<d>* const &n) {
                            return ebx + (n->kind() == Node<d>::K_Sensor ? 1 : 0);
                         };
                                    
         eog.fitness(std::accumulate(eog.cbegin(),eog.cend(),0.0,gridFold));
      }
   private:
      Grid<d> _grid;
};

#endif // __GRIDEVAL_H__
