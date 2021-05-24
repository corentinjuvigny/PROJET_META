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
 * @file gridMoUtils.h
 *
 * @brief utils functions for paradiseo algorithms
 *
 */

#ifndef __GRIDMOUTILS_H__
#define __GRIDMOUTILS_H__

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <algorithm>
#include <eo>
#include <limits>
#include <numeric>
#include <utility>
#include "gridInit.h"

typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS > graph_t;
typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_descriptor;

template <size_t d>
long connectedComponents(const Grid<d> &grid, const Node<d>* sensor)
{
   graph_t graph;
   std::map<Node<d>*,size_t,typename Node<d>::NodeCmp> nodeMap;
   size_t n = 0;
   std::for_each( grid.solution().cbegin()
                , grid.solution().cend()
                , [&nodeMap,&n,&sensor](Node<d>* node) {
                     if ( (node->kind() == Node<d>::K_Well || node->kind() == Node<d>::K_Sensor)
                        && !equal_coord<d>(sensor->coord(),node->coord()) )
                        nodeMap.insert(std::make_pair(node,n++));
                  } );
   for (Node<d>* node : grid.solution()) {
      if ( equal_coord<d>(node->coord(),sensor->coord()) )
         continue;
      for (Node<d>* neighbor : node->aux()) {
         if ( equal_coord<d>(neighbor->coord(),sensor->coord()) )
            continue;
         boost::add_edge(nodeMap[node],nodeMap[neighbor],graph);
      }
   }
   std::vector<size_t> component(boost::num_vertices(graph));

   return boost::connected_components(graph,&component[0]);
}

template <size_t d>
inline long connectedComponents(eoGridSolution<d> &solution, bool debug = false)
{
   graph_t graph;
   std::map<Node<d>*,size_t,typename Node<d>::NodeCmp> nodeMap;
   size_t n = 0;
   std::for_each( solution.cbegin()
                , solution.cend()
                , [&nodeMap,&n](Node<d>* node) {
                     if ( node->kind() == Node<d>::K_Well || node->kind() == Node<d>::K_Sensor )
                        nodeMap.insert(std::make_pair(node,n++));
                  } );
   for (Node<d>* sensor : solution) {
      for (Node<d>* neighbor : sensor->aux()) {
         boost::add_edge(nodeMap[sensor],nodeMap[neighbor],graph);
      }
   }
   std::vector<size_t> component(boost::num_vertices(graph));

   if (debug) {
      for (auto node : nodeMap) {
         std::cerr << node.first << " : " << component[node.second] << std::endl;
      }
   }

   return boost::connected_components(graph,&component[0]);
}

#endif // __GRIDMOUTILS_H__
