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

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <execution>
#include "kdtree.h"
#include "node.hpp"

template <size_t d = 2>
class Grid {
   public:
      using SNode = typename Node<d>::SNode;
      using AVLNodes = typename Node<d>::AVLNodes;
      Grid<d>() = default;
      Grid<d>( const std::vector<SNode> &nodes
             , struct kdtree *kdTree
             , const long cover
             , const double communication_radius
             , const double capture_radius );
      Grid<d>( const size_t n
             , const long cover
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
      const long &cover() const { return _cover; }
      const double &communication_radius() const { return _communication_radius; }
      const double &capture_radius() const { return _capture_radius; }
      const SNode &well() const { return _nodes.front(); }
      void insertNode(SNode &&n);
      void insertNodeInSolution(Node<d>* &n);
      void add_coverage(const long coverage) { _cover -= coverage; }
      void remove_coverage(const long coverage) { _cover += coverage; }
      bool all_nodes_are_covered() const { return _cover <= 0; }
      void add_sensor_to_solution(Node<d>* const target);
      void remove_sensor_to_solution(Node<d>* const sensor);
      const AVLNodes& targets_in_neighbourhood() const;
      const AVLNodes& sensors_in_neighbourhood() const;
      std::optional<size_t> objective_value() const;
      void clear_solution() { _solution.clear(); }
      int run_dfs(const Node<d>* initial_node()) const;
      void finish();
      void maj( Node<d>* &selected_target
              , typename Node<d>::Queue &sensor_queue
              , typename Node<d>::Queue &visited_target_queue
              , const size_t new_covered_target_max );
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
      long               _cover;
      double             _communication_radius;
      double             _capture_radius;
};

template <size_t d>
Grid<d>::Grid( const std::vector<typename Node<d>::SNode> &nodes
             , struct kdtree *kdTree
             , const long cover
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
             , const long cover
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
   auto nw = std::move(n);
   _nodes.push_back(nw);
   kd_insert(_kdTree,nw->coord().data(),nw.get());
}

template <size_t d>
void Grid<d>::insertNodeInSolution(Node<d>* &n)
{
   _solution.insert(n);
}

enum class RadiusType { CAPTURE, COMMUNICATION };

template <size_t d, RadiusType t>
typename Node<d>::Queue set_capture_and_communication_queue(const Grid<d> *g, const typename Node<d>::SNode &n)
{
   const double radius = t == RadiusType::CAPTURE ? g->capture_radius() : g->communication_radius();
   struct kdres *covered_node_in_radius_list;
   Node<d>* node_in_radius;
   typename Node<d>::Queue queue;
   covered_node_in_radius_list = kd_nearest_range(g->kdTree(),n->coord().data(),radius);
   while (!kd_res_end(covered_node_in_radius_list)) {
      node_in_radius = (Node<d>*)kd_res_item(covered_node_in_radius_list,NULL);
      queue.push_front(node_in_radius);
      kd_res_next(covered_node_in_radius_list);
   }
   kd_res_free(covered_node_in_radius_list);
   return queue;
}

template <size_t d>
void Grid<d>::finish()
{
   std::for_each( std::execution::par_unseq
                , _nodes.begin()
                , _nodes.end()
                , [this](SNode &node) {
                     node->set_capture_queue(set_capture_and_communication_queue<d,RadiusType::CAPTURE>(this,node));
                     node->set_communication_queue(set_capture_and_communication_queue<d,RadiusType::COMMUNICATION>(this,node));
                  });
}

template <size_t d>
void Grid<d>::maj( Node<d>* &selected_target
                 , typename Node<d>::Queue &sensor_queue
                 , typename Node<d>::Queue &visited_target_queue
                 , const size_t new_covered_target_max )
{
   selected_target->set_new_sensor(sensor_queue);
   this->add_coverage(new_covered_target_max);
   selected_target->set_sensor_new_communication(sensor_queue);
   selected_target->set_targets_new_capture_sensor(visited_target_queue); 
}

template <size_t d>
std::optional<size_t> Grid<d>::objective_value() const
{
   if ( _solution.empty() )
      return std::nullopt;
   return std::make_optional(_solution.size() - 1);
}

template <size_t d>
void Grid<d>::add_sensor_to_solution(Node<d>* const sensor)
{
   if ( sensor->kind() != Node<d>::K_Target )
      return;
   this->add_coverage(sensor->set_as_sensor());
   _solution.insert(sensor);
}

template <size_t d>
void Grid<d>::remove_sensor_to_solution(Node<d>* const sensor)
{
   if ( sensor->kind() != Node<d>::K_Sensor )
      return;
   this->remove_coverage(sensor->set_as_target());
   _solution.erase(sensor);
}

#endif // __GRID_HPP__
