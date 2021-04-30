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

/** @file greedy.h
 *
 * @brief Greedy algorithm to construct a viable solution of the problem
 *
 */

#ifndef __GREEDY_H__
#define __GREEDY_H__

#include "grid.hpp"
#include <algorithm>

void greedy_construction_2D(Grid<2> &g);

template <size_t d>
using AVLVisitedNode = std::map<std::string,typename Node<d>::Queue>;

template <size_t d>
void find_best_target( const typename Node<d>::SNode &node
                     , typename Node<d>::SNode *selected_target
                     , typename Node<d>::Queue *visited_target_queue
                     , size_t *new_covered_target_max
                     , AVLVisitedNode<d> *visited_target_avl )
{
#if DEBUG
   std::cerr << "### ON TRAITE ###\n"
             << *node
             << std::endl;
#endif
   size_t visit_node = 0, new_covered_target;
   const typename Node<d>::Queue &target_in_radius_queue = node->communication_queue();

   /* We iterate over all of targets */
   for (auto &target_in_radius : target_in_radius_queue) {
      if ( (!equal_coord(node->coord(),target_in_radius->coord()))
           && target_in_radius->kind() == Node<d>::K_Target )
         visit_node = 0;
      new_covered_target = 0;

      /* If we use an AVL */
      if ( visited_target_avl != NULL ) {
         /* We look if we have already visited the current target*/
         auto point_queue = visited_target_avl->find(target_in_radius->name());
         /* We never have visited the current target */
         if ( point_queue == visited_target_avl->end() ) {
            /* We add the target in the avl (its name is the key), the content is 
             * a steck in which we put  the point */
            typename Node<d>::Queue node_queue;
            node_queue.push_front(node);
            visited_target_avl->insert(std::make_pair(node->name(),node_queue));
         } else {
            point_queue->second.push_front(node);
            visit_node = 1;
         }

      }
      if ( visit_node == 0 ) {
         const typename Node<d>::Queue &covered_targets_in_radius_queue = target_in_radius->capture_queue();
         for (const auto &covered_target_in_radius : covered_targets_in_radius_queue) {
            if ( (!equal_coord(target_in_radius->coord(),covered_target_in_radius->coord()))
                 && (covered_target_in_radius->kind() == Node<d>::K_Target) ) {
               if ( covered_target_in_radius->aux().empty() )
                  new_covered_target++;
            }

         }
         if ( target_in_radius->aux().empty() )
            new_covered_target++;
         if ( new_covered_target >= *new_covered_target_max ) {
            *selected_target = target_in_radius;
            *visited_target_queue = covered_targets_in_radius_queue;
            *new_covered_target_max = new_covered_target;
         }
      }

   }
                        
}

template <size_t d>
inline void greedy_construction(Grid<d> &g)
{
   std::cerr << "Error greedy_construction<" << d << "> not yet implemented" << std::endl;
   typename Node<d>::SNode well_node = g.nodes().front();

   typename Node<d>::SNode selected_target;
   typename Node<d>::Queue visited_target_queue;
   size_t new_covered_target_max = 0;
   AVLVisitedNode<d> visited_target_avl;

   find_best_target<d>(well_node,&selected_target,&visited_target_queue,&new_covered_target_max,NULL);

   g.insertNodeInSolution(well_node);
   g.insertNodeInSolution(selected_target);

   typename Node<d>::Queue empty_queue;
   empty_queue.push_back(well_node);

   g.maj(selected_target,empty_queue,visited_target_queue,new_covered_target_max);

   while (g.cover() > 0) {
      std::for_each( g.solution().cbegin()
                   , g.solution().cend()
                   , [&](auto &current_sensor_map)
                     { find_best_target<d>(current_sensor_map.second,&selected_target,&visited_target_queue,&new_covered_target_max,&visited_target_avl); });
      g.insertNodeInSolution(selected_target);
      auto sensor_queue = visited_target_avl.find(selected_target->name())->second;
      g.maj(selected_target,sensor_queue,visited_target_queue,new_covered_target_max);
   }

   
}

#endif // __GREEDY_H__
