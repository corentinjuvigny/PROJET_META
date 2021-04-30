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

#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include "kdtree.h"
#include "node.hpp"

template <size_t d>
class Grid {
   public:
      using SNode = typename Node<d>::SNode;
      using AVLNodes = typename Node<d>::AVLNodes;
      Grid<d>( const std::vector<SNode> &nodes
             , struct kdtree *kdTree
             , const unsigned long cover
             , const double communication_radius
             , const double capture_radius );
      Grid<d>( const size_t n
             , const unsigned long cover
             , const double communication_radius
             , const double capture_radius );
      Grid<d>(const Grid<d> &) = default;
      Grid<d>(Grid<d> &&);
      ~Grid() = default;
      void end() { kd_free(_kdTree); }
      const std::vector<SNode> &nodes() const { return _nodes; }
      size_t nbNodes() const { return _nodes.size(); }
      struct kdtree *kdTree() const { return _kdTree; }
      const AVLNodes &solution() const { return _solution; }
      const unsigned long &cover() const { return _cover; }
      const double &communication_radius() const { return _communication_radius; }
      const double &capture_radius() const { return _capture_radius; }
      void insertNode(SNode &&n);
      void insertNodeInSolution(SNode &n);
      void set_cover(const size_t new_covered_target_max) { _cover -= new_covered_target_max; }
      void maj( SNode &selected_target
              , std::list<SNode> &sensor_queue
              , std::list<SNode> &visited_target_queue
              , const int new_covered_target_max );
      friend std::ostream& operator<<(std::ostream &os, const Grid<d> &g)
      {
         if ( g.nbNodes() == 0 )
            os << "========= Grid is empty =========";
         else
            os << "============= Grid =============\n";
         for (auto &n : g._nodes) {
            os << "######## NODES ########\n"
               << *n
               << "\n#######################\n";
         }
         return os;
      }

   private:
      std::vector<SNode> _nodes;
      struct kdtree *    _kdTree;
      AVLNodes           _solution;
      unsigned long      _cover;
      double             _communication_radius;
      double             _capture_radius;
};

template <size_t d>
Grid<d>::Grid( const std::vector<typename Node<d>::SNode> &nodes
             , struct kdtree *kdTree
             , const unsigned long cover
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
             , const unsigned long cover
             , const double communication_radius
             , const double capture_radius )
   : _nodes(), _kdTree(NULL), _solution(), _cover(cover)
   , _communication_radius(communication_radius)
   , _capture_radius(capture_radius)
{
   _nodes.reserve(n);
   _kdTree = kd_create(static_cast<int>(d));
   kd_data_destructor(_kdTree,NULL);
}
template <size_t d>
Grid<d>::Grid(Grid<d> &&g)
   : _nodes(std::move(g._nodes))
   , _kdTree(std::move(g._kdTree))
   , _solution(std::move(g._solution))
   , _cover(std::move(g._cover))
   , _communication_radius(std::move(g._communication_radius))
   , _capture_radius(std::move(g._capture_radius))
{ }

template <size_t d>
void Grid<d>::insertNode(typename Grid<d>::SNode &&n)
{
   _nodes.push_back(n); 
   kd_insert(_kdTree,n->coord().data(),n.get());
}

template <size_t d>
void Grid<d>::insertNodeInSolution(typename Grid<d>::SNode &n)
{
   _solution.insert(std::make_pair(n->name(),n));
}

template <size_t d>
void Grid<d>::maj( SNode &selected_target
                 , std::list<SNode> &sensor_queue
                 , std::list<SNode> &visited_target_queue
                 , const int new_covered_target_max )
{
   selected_target->set_new_sensor(sensor_queue);
   this->set_cover(new_covered_target_max);
   selected_target->set_sensor_new_communication(sensor_queue);
   selected_target->set_target_new_capture_sensor(visited_target_queue); 
}

#endif // __GRID_HPP__
