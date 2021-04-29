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
#include <vector>
#include <memory>
#include <tuple>

template <size_t d>
class Node {
   public:
      typedef std::array<double,d> Coord;
      typedef std::shared_ptr<Node<d>> SNode;
      enum Kind { K_Well, K_Target, K_Sensor };
      Node<d>(const Kind kind,const std::string &name, const Coord &coord)
         :  _kind(kind), _name(name), _coord(coord) { }
      Node(const Node &node) = default;
      Node(Node &&node) = default;
      ~Node() = default;
      const Kind &kind() const { return _kind; }
      const std::string &name() const { return _name; }
      const Coord &coord() const { return _coord; }
      const std::list<SNode> &capture_queue() const { return _capture_queue; }
      const std::list<SNode> &communication_queue() const { return _communication_queue; }
      const std::map<SNode,SNode> &aux() const { return _aux; }
      friend std::ostream& operator<<(std::ostream &os, const Node &n)
      {
         os << n._name << " ( ";
         for (auto &e : n._coord)
            os << e << ' ';
         os << ") of type " << n._kind;
         return os;
      }
      /**
       * Compare two nodes using their names
       *
       * @param na A node
       * @param nb A node
       *
       * @return An integer given by string::compare
       */
      static inline int compare(const Node &na, const Node &nb) 
      {
         return na._name.compare(nb._name);
      }
      
   private:
      Kind                  _kind;
      std::string           _name;
      Coord                 _coord;
      std::list<SNode>      _capture_queue;
      std::list<SNode>      _communication_queue;
      std::map<SNode,SNode> _aux;
};

template <int d>
class WellNode : public Node<d> {
   public:
      WellNode(const std::string &name, const typename Node<d>::Coord &coord)
         : Node<d>(Node<d>::K_Well,name,coord) { }
};

template <int d>
class TargetNode : public Node<d> {
   public:
      TargetNode(const std::string &name, const typename Node<d>::Coord &coord)
         : Node<d>(Node<d>::K_Target,name,coord) { }
};

template <int d>
class SensorNode : public Node<d> {
   public:
      SensorNode(const std::string &name, const typename Node<d>::Coord &coord)
         : Node<d>(Node<d>::K_Sensor,name,coord) { }
};

#endif //__NODE_HPP__
