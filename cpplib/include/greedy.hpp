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

#define DEBUG 0

#ifndef __GREEDY_HPP__
#define __GREEDY_HPP__

#include "grid.hpp"
#include <algorithm>

template <size_t d>
using AVLVisitedNode = std::map<std::string,typename Node<d>::Queue>;

template <size_t d>
void find_best_target( Node<d> *node
                     , Node<d> **selected_target
                     , typename Node<d>::Queue *visited_target_queue
                     , size_t *new_covered_target_max
                     , AVLVisitedNode<d> *visited_target_avl = NULL )
{
#if DEBUG
   std::cerr << "### ON TRAITE ###\n"
             << *node
             << std::endl;
#endif
   bool visit_node = false;
   size_t new_covered_target;
   const typename Node<d>::Queue &target_in_radius_queue = node->communication_queue();
#if DEBUG
   std::cerr << "target_in_radius_queue.size() = "
             << target_in_radius_queue.size()
             << std::endl;
#endif

   /* We iterate over all of targets */
   for (auto &target_in_radius : target_in_radius_queue) {
      if ( (!equal_coord(node->coord(),target_in_radius->coord()))
           && target_in_radius->kind() == Node<d>::K_Target ) {
         visit_node = false;
         new_covered_target = 0;

         /* If we use an AVL */
         if ( visited_target_avl != NULL ) {
            /* We look if we have already visited the current target*/
            auto point_queue = visited_target_avl->find(target_in_radius->name());
            /* We never have visited the current target */
            if ( point_queue == visited_target_avl->end() ) {
               /* We add the target in the avl (its name is the key), the content is 
                * a stack in which we put the point */
               typename Node<d>::Queue node_queue;
               node_queue.push_front(node);
               visited_target_avl->insert(std::make_pair(target_in_radius->name(),node_queue));
            } else {
#if DEBUG
               std::cerr << "On ne poursuit pas " << new_covered_target << std::endl;
#endif
               point_queue->second.push_front(node);
               visit_node = true;
            }

         }
         if ( !visit_node ) {
#if DEBUG
            std::cerr << "On poursuit " << new_covered_target << std::endl;
#endif
            const typename Node<d>::Queue &covered_targets_in_radius_queue = target_in_radius->capture_queue();
            for (const auto &covered_target_in_radius : covered_targets_in_radius_queue) {
               if ( (!equal_coord(target_in_radius->coord(),covered_target_in_radius->coord()))
                    && (covered_target_in_radius->kind() == Node<d>::K_Target) ) {
                  if ( covered_target_in_radius->aux().empty() ) {
#if DEBUG
                     std::cerr << "=> new" << std::endl;
#endif
                     new_covered_target++;
                  }
               }
            }
#if DEBUG
            std::cerr << "new_cover_target : " << new_covered_target << std::endl;
#endif
            if ( target_in_radius->aux().empty() )
               new_covered_target++;
            if ( new_covered_target >= *new_covered_target_max ) {
#if DEBUG
               std::cerr << "Target in radius : " << *target_in_radius << std::endl;
#endif
               *selected_target = target_in_radius;
               *visited_target_queue = covered_targets_in_radius_queue;
               *new_covered_target_max = new_covered_target;
            }
         }
      }
   }
}

template <size_t d>
inline void greedy_construction(Grid<d> &g)
{
   Node<d>* well_node = g.nodes().front().get();

   Node<d>* selected_target;
   typename Node<d>::Queue visited_target_queue;
   size_t new_covered_target_max = 0;
   AVLVisitedNode<d> *visited_target_avl;

   find_best_target<d>(well_node,&selected_target,&visited_target_queue,&new_covered_target_max);

   g.insertNodeInSolution(well_node);
   g.insertNodeInSolution(selected_target);
#if DEBUG
      std::cerr << "Well Inserted " << std::endl;
#endif

   typename Node<d>::Queue empty_queue;
   empty_queue.push_back(well_node);

   g.maj(selected_target,empty_queue,visited_target_queue,new_covered_target_max);

   while (!g.all_nodes_are_covered()) {
#if DEBUG
      std::cerr << "Need to be covered : " << g.cover() << std::endl;
#endif 
      visited_target_avl = new AVLVisitedNode<d>();
      new_covered_target_max = 0;
      std::for_each( g.solution().cbegin()
                   , g.solution().cend()
                   , [&](auto &current_sensor_map)
                     { find_best_target<d>( current_sensor_map.second
                                          , &selected_target
                                          , &visited_target_queue
                                          , &new_covered_target_max
                                          , visited_target_avl); } );
      g.insertNodeInSolution(selected_target);
      auto sensor_queue_avl = visited_target_avl->find((selected_target)->name());
      typename Node<d>::Queue sensor_queue = sensor_queue_avl != visited_target_avl->end()
                                             ? sensor_queue_avl->second
                                             : typename Node<d>::Queue();
      g.maj( selected_target
           , sensor_queue
           , visited_target_queue
           , new_covered_target_max ); 
      delete visited_target_avl;
   }
}

#endif // __GREEDY_HPP__
