/*

copyright (c) 2021, adrien blassiau and corentin juvigny

permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

the software is provided "as is" and the author disclaims all
warranties with regard to this software including all implied
warranties of merchantability and fitness. in no event shall the
author be liable for any special, direct, indirect, or
consequential damages or any damages whatsoever resulting from
loss of use, data or profits, whether in an action of contract,
negligence or other tortious action, arising out of or in
connection with the use or performance of this software.

*/

/** @file mip.hpp
 *
 * @brief Mixed integer algorithm to construct the optimal solution of the problem
 *
 */
#ifndef __MIP_HPP__
#define __MIP_HPP__

#include <grid.hpp>
#include <ios>
#include <sstream>

template <size_t d>
void mip_resolution(Grid<d>&)
{
   std::cout << "MIP not yet defined for dimension " << d << " problem" << std::endl;
}

void mip_resolution_2D(Grid<2> &g);

template <>
inline void mip_resolution(Grid<2> &g)
{
   mip_resolution_2D(g);
}

#endif //__MIP_HPP__
