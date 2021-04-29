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

/** @file grid.hpp
 *
 * @brief grid structure containig the nodes, templated over the dimension d of
 *        nodes
 *
 */

#ifndef __GRID_HPP__
#define __GRID_HPP__

#include <map>
#include <vector>
#include "kdtree.h"
#include "node.hpp"

template <size_t d>
class Grid {
   public:
      using SNode = typename Node<d>::SNode;
      typedef std::map<std::string,SNode> AVLNodes;
      Grid<d>( const std::vector<SNode> &nodes
          , struct kdtree *kdTree
          , const unsigned cover
          , const double communication_radius
          , const double capture_radius );
      Grid<d>( const size_t n
          , const unsigned cover
          , const double communication_radius
          , const double capture_radius );
      Grid<d>(const Grid<d> &) = default;
      ~Grid();
      const std::vector<SNode> &nodes() const { return _nodes; }
      size_t nbNodes() const { return _nodes.size(); }
      struct kdtree *kdTree() const { return _kdTree; }
      const AVLNodes &solution() const { return _solution; }
      const unsigned &cover() const { return _cover; }
      const double &communication_radius() const { return _communication_radius; }
      const double &capture_radius() const { return _capture_radius; }
      friend std::ostream& operator<<(std::ostream &os, const Grid<d> &g)
      {
         if ( g.nbNodes() == 0 )
            os << "========= Grid is empty =========";
         else
            os << "============= Grid =============\n";
         for (auto &n : g._nodes) {
            os << "######## NODES ########\n"
               << n
               << "#######################\n";
         }
         return os;
      }
   private:
      std::vector<SNode> _nodes;
      struct kdtree *    _kdTree;
      AVLNodes           _solution;
      unsigned           _cover;
      double             _communication_radius;
      double             _capture_radius;
};

template <size_t d>
Grid<d>::Grid( const std::vector<typename Node<d>::SNode> &nodes
             , struct kdtree *kdTree
             , const unsigned cover
             , const double communication_radius
             , const double capture_radius )
   : _nodes(nodes), _kdTree(NULL), _solution(), _cover(cover)
   , _communication_radius(communication_radius)
   , _capture_radius(capture_radius)
{
   _kdTree = kdTree;
}

template <size_t d>
Grid<d>::Grid( const size_t n
             , const unsigned cover
             , const double communication_radius
             , const double capture_radius )
   : _nodes(), _kdTree(NULL), _solution(), _cover(cover)
   , _communication_radius(communication_radius)
   , _capture_radius(capture_radius)
{
   _nodes.reserve(n);
   _kdTree = kd_create(static_cast<int>(d));
}

template <size_t d>
Grid<d>::~Grid() 
{
   kd_free(_kdTree);
}

#endif // __GRID_HPP__
