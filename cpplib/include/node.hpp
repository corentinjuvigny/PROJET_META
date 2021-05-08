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

/** @file node.hpp
 *
 * @brief class to manage a node of dimension d
 *
 */

#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <ostream>
#include <string>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>
#include <memory>
#include <execution>
#include <algorithm>

template <size_t d>
class Node {
   public:
      /* 
       * @struct NodeCmp
       *
       * @brief struct needed for the comparaison between the nodes
       */
      struct NodeCmp {
         bool operator()(const Node<d>* const &lhs, const Node<d>* const &rhs) const
         {
            return compareNode(lhs,rhs);
         }
      };
      /*
       * @enum Kind
       *
       * @brief represents the type of the node
       */
      enum Kind { K_Well, K_Target, K_Sensor };
      typedef std::set<Node<d>*,NodeCmp> AVLNodes;
      typedef std::array<double,d> Coord;
      typedef std::shared_ptr<Node<d>> SNode;
      typedef std::list<Node<d>*> Queue;
      Node<d>(const Kind kind, const std::string &name, const Coord &coord)
         :  _kind(kind), _name(name), _coord(coord) { }
      Node(const Node &node) = default;
      Node(Node &&node) = default;
      ~Node() = default;
      const Kind &kind() const { return _kind; }
      const std::string &name() const { return _name; }
      const Coord &coord() const { return _coord; }
      const Queue &capture_queue() const { return _capture_queue; }
      const Queue &communication_queue() const { return _communication_queue; }
      const AVLNodes &aux() const { return _aux; }
      void set_capture_queue(Queue &q) { _capture_queue = q; }
      void set_capture_queue(Queue &&q) { _capture_queue = q; }
      void set_communication_queue(Queue &q) { _communication_queue = q; }
      void set_communication_queue(Queue &&q) { _communication_queue = q; }
      void set_new_sensor(Queue &sensor_queue);
      void set_sensor_new_communication(Queue &sensor_queue);
      void set_target_new_capture_sensor(Queue &visited_target_queue);
      friend std::ostream& operator<<(std::ostream &os, const Node &n)
      {
         os << n._name << " ( ";
         std::for_each(n._coord.cbegin(),n._coord.cend(),[&os](const auto &e) { os << e << ' '; });
         os << ") of type " << n._kind;
         return os;
      }
      /**
       * Compare two nodes using their names
       *
       * @param na A node
       * @param nb A node
       *
       * @return A boolean satisfiying the Compare requirements
       */
      static inline bool compareNode(const Node* const &na, const Node* const &nb) 
      {
         return na->_name < nb->_name;
      }
      
   private:
      Kind           _kind;
      std::string    _name;
      Coord          _coord;
      Queue          _capture_queue;
      Queue          _communication_queue;
      AVLNodes       _aux;
};

template <size_t d>
class WellNode : public Node<d> {
   public:
      WellNode(const std::string &name, const typename Node<d>::Coord &coord)
         : Node<d>(Node<d>::K_Well,name,coord) { }
};

template <size_t d>
class TargetNode : public Node<d> {
   public:
      TargetNode(const std::string &name, const typename Node<d>::Coord &coord)
         : Node<d>(Node<d>::K_Target,name,coord) { }
};

template <size_t d>
class SensorNode : public Node<d> {
   public:
      SensorNode(const std::string &name, const typename Node<d>::Coord &coord)
         : Node<d>(Node<d>::K_Sensor,name,coord) { }
};

template <size_t d>
constexpr bool equal_coord(const typename Node<d>::Coord &ca, const typename Node<d>::Coord &cb)
{
   return std::equal(std::execution::par,ca.cbegin(),ca.cend(),cb.cbegin(),cb.cend());
}

template <>
constexpr bool equal_coord<2>(const typename Node<2>::Coord &ca, const typename Node<2>::Coord &cb)
{
   auto [xa,ya] = ca;
   auto [xb,yb] = cb;
   return xa == xb && ya == yb;
}

template <size_t d>
void Node<d>::set_new_sensor(Queue &sensor_queue)
{
   if ( this->_kind == K_Sensor || this->_kind == K_Well )
      return;
   AVLNodes avl;
   std::for_each( sensor_queue.cbegin()
                , sensor_queue.cend()
                , [&,this](const auto &sensor) 
                  {
                     if ( (!equal_coord<d>(this->coord(),sensor->coord()))
                           && (sensor->kind() == K_Well || sensor->kind() == K_Sensor) )
                        avl.insert(sensor);
                  } );
   this->_kind = K_Sensor;
   this->_aux = avl;
}

template <size_t d>
void Node<d>::set_sensor_new_communication(Queue &sensor_queue)
{
   std::for_each( std::execution::par_unseq
                , sensor_queue.begin()
                , sensor_queue.end()
                , [&,this](auto &sensor)
                  {
                     if ( (!equal_coord<d>(this->coord(),sensor->coord()))
                           && (sensor->kind() == K_Well || sensor->kind() == K_Sensor) )
                        sensor->_aux.insert(this);
                  } );
}

template <size_t d>
void Node<d>::set_target_new_capture_sensor(Queue &visited_target_queue)
{
   std::for_each( std::execution::par_unseq
                , visited_target_queue.begin()
                , visited_target_queue.end()
                , [&,this](auto &target)
                  {
                     if ( (!equal_coord<d>(this->coord(),target->coord()))
                           && (target->kind() == K_Target) ) {
                        target->_aux.insert(this);
                     }
                  } );
}

#endif //__NODE_HPP__
