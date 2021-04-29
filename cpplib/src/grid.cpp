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

#include "grid.hpp"
#if 0
template <int k>
Grid<k>::Grid( const std::vector<Node::SNode> &nodes
          , struct kdtree *kdTree
          , const int cover
          , const double communication_radius
          , const double capture_radius )
   : _nodes(nodes), _kdTree(NULL), _solution(), _cover(cover)
   , _communication_radius(communication_radius)
   , _capture_radius(capture_radius)
{
   _kdTree = kdTree;
}

template <int k>
Grid<k>::Grid( const size_t n
          , const int d
          , const int cover
          , const double communication_radius
          , const double capture_radius )
   : _nodes(), _kdTree(NULL), _solution(), _cover(cover)
   , _communication_radius(communication_radius)
   , _capture_radius(capture_radius)
{
   _nodes.reserve(n);
   _kdTree = kd_create(d);
}

template <int k>
Grid<k>::~Grid() 
{
   kd_free(_kdTree);
}

template <int k>
std::ostream& operator<<(std::ostream& os, const Grid<k>& g)
{
   if ( g.nbNodes() == 0 )
      os << "========= Grid is empty =========";
   else
      os << "========= Grid =========\n";
   for (auto &n : g._nodes) {
      os << "######## NODES ########\n"
         << n
         << "#######################\n";
   }
   return os;
}
#endif
