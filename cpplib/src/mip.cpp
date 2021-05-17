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

/** @file mip.cpp
 *
 * @brief Mixed integer algorithm to construct the optimal solution of the problem
 *
 */

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <algorithm>
#include <mip.hpp>
#include <utility>
#include <ilcplex/ilocplex.h>

#define EPSILON 10e-10

typedef std::map<Node<2>*,std::pair<IloNum,long>,Node<2>::NodeCmp> IloNumIndexMap;
typedef std::map<Node<2>*,IloNumVar,Node<2>::NodeCmp> IloNumVarMap;
typedef std::map<Node<2>*,IloNum,Node<2>::NodeCmp> IloNumMap;
typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS > graph_t;
typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_descriptor;

ILOLAZYCONSTRAINTCALLBACK1( ValidityCallback
                          , const IloNumVarMap&
                          , X )
{
   IloNumIndexMap Xr;
   size_t index = 0;
#if 0
   for (const auto &[name,value] : X)
      Xr[name] = std::make_pair(getValue(value),index++);
#else
   for (const auto &[node,value] : X) {
      IloInt x = getValue(value);
      if ( abs(1 - x) <= EPSILON )
         Xr[node] = std::make_pair(x,index++);
      else
         Xr[node] = std::make_pair(x,-1);
   }
#endif

   graph_t graph;
#if 0
   for (struct {size_t i; IloNumIndexMap::const_iterator it;} ist = {0, Xr.cbegin()}; ist.it != Xr.cend(); ist.it++, ist.i++) {
      for (struct {size_t j; IloNumIndexMap::const_iterator jt;} jst = {ist.i, ist.it}; jst.jt != Xr.cend(); jst.jt++, jst.j++) {
         if ( abs(ist.it->second.first - 1) <= 10e-10
            && (std::find( ist.it->first->communication_queue().cbegin()
                         , ist.it->first->communication_queue().cend()
                         , jst.jt->first ) != ist.it->first->communication_queue().end()) ) {
            boost::add_edge(ist.i,jst.j,graph);
         }
      }
   }
#endif
#if 1
   for (const auto &[node,pair] : Xr) {
      const auto [value1,index1] = pair;
      if ( abs(value1 - 1) <= EPSILON ) {
         for (const auto &neighbour : node->communication_queue()) {
            const auto [value2,index2] = Xr[neighbour];
            if ( abs(value2 - 1) <= EPSILON )
               boost::add_edge(index1,index2,graph);
         }
      }
   }
#endif
#if 0
   std::vector<Node<2>*> nodes(X.size());
   std::transform( Xr.cbegin()
                 , Xr.cend()
                 , nodes.begin()
                 , [](const auto &node) { return node.first; });
   for (size_t i = 0; i < nodes.size(); i++) {
      for (size_t j = i+1; j < nodes.size(); j++) {
         if ( Xr[nodes[i]].value == 1
            && (std::find( nodes[i]->communication_queue().cbegin()
                         , nodes[i]->communication_queue().cend()
                         , nodes[j] ) != nodes[i]->communication_queue().end()) ) {
            boost::add_edge(i,j,graph);
         }
      }
   }

#endif

   std::vector<size_t> component(boost::num_vertices(graph));
   size_t num = boost::connected_components(graph,&component[0]);

   if ( num > 1 ) { // Graph is disconnected
      IloExpr e(getEnv());
      IloInt nbr = 0;
      for (const auto &[node,tuple] : Xr) {
         if ( tuple.first == 1 ) {
            e += X.find(node)->second; 
            nbr++;
         } else {
            //e -= X.find(node)->second;
         }
      }
      addLocal(e <= nbr-1);
      e.end();
   }
}

void mip_resolution_2D(Grid<2> &g)
{      
   IloEnv env;
   
   IloModel model(env);

   // Decision var
   IloNumVarMap X;
   for (const auto &s : g.nodes()) { 
      std::stringstream ss;
      ss << "x_" << s->name();
      X[s.get()] = IloNumVar(env,0,1,ILOBOOL,ss.str().c_str());
   }
   
   // Objective
   IloExpr obj(env);
   for (auto &x_s : X)
      obj += x_s.second;
   model.add(IloMinimize(env,obj,"obj"));
   obj.end();

   // Constraints
   for (const auto &s : g.nodes()) {
      IloExpr e_com(env);
      IloExpr e_capt(env);
      for (const auto &v : s->communication_queue())
         e_com += X[v];
      for (const auto &v : s->capture_queue())
         e_capt += X[v];
      model.add(e_com >= 1);
      model.add(e_capt >= 1);
      e_capt.end();
      e_com.end();
   }
   model.add(X[g.well().get()] == 1);

   IloCplex cplex(model);

   cplex.setParam(IloCplex::Param::Threads,8);
   cplex.setParam(IloCplex::Param::MIP::Strategy::File,1);
   cplex.setParam(IloCplex::Param::MIP::Limits::Solutions,1000);
   cplex.use(ValidityCallback(env,X));
   cplex.solve();

   IloAlgorithm::Status status = cplex.getStatus();
   std::stringstream ss; 
   ss << "Solution status:                   " << cplex.getStatus();
   std::string separator_line(ss.str().length(),'-');
   std::cout << separator_line << std::endl;
   std::cout << ss.str() << std::endl;
   IloNumIndexMap Xr;
   graph_t graph;
   size_t index = 0;
   switch ( status ) {
      case IloAlgorithm::Optimal:
         std::cout << "Nodes processed:                   " << cplex.getNnodes() << std::endl;
         std::cout << "Active user cuts/lazy constraints: " << cplex.getNcuts(IloCplex::CutUser) << std::endl;
         std::cout << "Time elapsed:                      " << cplex.getTime() << std::endl;
         std::cout << "Optimal value:                     " << cplex.getObjValue()-1 << std::endl;
         for (const auto &[node,value] : X) {
            IloInt x = cplex.getValue(value);
            if ( abs(1 - x) <= EPSILON )
               Xr[node] = std::make_pair(x,index++);
            else
               Xr[node] = std::make_pair(x,-1);
         }
         for (const auto &v : g.nodes()) {
            if ( Xr[v.get()].first == 1 ) {
               typename Node<2>::Queue queue;
               for (const auto &node : v->communication_queue())
                  if ( Xr[node].first == 1 ) {
                     queue.push_front(node);
                  }
               v->set_new_sensor(queue);
            }
         }
         {
            for (const auto &[node,pair] : Xr) {
               const auto [value1,index1] = pair;
               if ( abs(value1 - 1) <= EPSILON ) {
                  for (const auto &neighbour : node->communication_queue()) {
                     const auto [value2,index2] = Xr[neighbour];
                     if ( abs(value2 - 1) <= EPSILON ) {
                        boost::add_edge(index1,index2,graph);
                        std::cout << "Node "<<*node<<" connected to node "<<*neighbour<< " with indices : " << index1 << " " << index2 << std::endl;
                     }
                  }
               }
            }
            std::vector<size_t> component(boost::num_vertices(graph));
            long num = boost::connected_components(graph,&component[0]);
            std::cout << "Number of connected components " << num << std::endl;
            std::cout << "Number of vertices in the graph : " << boost::num_vertices(graph) << std::endl;

            for (const auto &[node,pair] : Xr) {
               const auto [value1,index1] = pair;
               if ( abs(value1 - 1) <= EPSILON ) {
                  std::cout << "Node " << *node << " in component " << component[index1] << std::endl;
               }
            }

         }
         break;
      default:
         break;
   }
   std::cout << separator_line << std::endl;

   model.end();
   cplex.end();
   env.end();
}
