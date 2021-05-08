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

#include <algorithm>
#include <mip.hpp>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <utility>

typedef std::map<Node<2>*,IloNumVar,Node<2>::NodeCmp> IloNumVarMap;
typedef std::map<Node<2>*,IloNum,Node<2>::NodeCmp> IloNumMap;
typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS > graph_t;
typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_descriptor;

ILOLAZYCONSTRAINTCALLBACK2( ValidityCallback
                          , const IloNumVarMap&
                          , X
                          , const Grid<2>&
                          , g )
{
   IloNumMap Xr;
   for (const auto &[node,value] : X)
      Xr[node] = getValue(value);
   std::vector<Node<2>*> nodes(g.nbNodes());
   std::transform( 
                  Xr.cbegin()
                 , Xr.cend()
                 , nodes.begin()
                 , [&](const auto &node) { return node.first; });

   graph_t graph;
   for (size_t i = 0; i < nodes.size(); i++) {
     for (size_t j = i+1; j < nodes.size(); j++) {
        if ( Xr[nodes[i]] == 1
           && (std::find( nodes[i]->communication_queue().cbegin()
                        , nodes[i]->communication_queue().cend()
                        , nodes[j] ) != nodes[i]->communication_queue().end()) ) {
           boost::add_edge(i,j,graph);
        }
     }
   }

   std::vector<size_t> component(boost::num_vertices(graph));
   size_t num = boost::connected_components(graph,&component[0]);

   if ( num > 1 ) { // Graph is disconnected
      IloExpr e(getEnv());
      IloInt nbr = 0;
      for (const auto &[node,value] : Xr) {
         if ( value == 1 ) {
           e += X.find(node)->second; 
           nbr++;
         } else {
            //e -= X.find(node)->second;
         }
      }
      add(e <= nbr-1);
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
   cplex.use(ValidityCallback(env,X,g));
   cplex.solve();

   IloAlgorithm::Status status = cplex.getStatus();
   std::stringstream ss; 
   ss << "Solution status:                   " << cplex.getStatus();
   std::string separator_line(ss.str().length(),'-');
   std::cout << separator_line << std::endl;
   std::cout << ss.str() << std::endl;
   IloNumMap Xr;
   switch ( status ) {
      case IloAlgorithm::Optimal:
         std::cout << "Nodes processed:                   " << cplex.getNnodes() << std::endl;
         std::cout << "Active user cuts/lazy constraints: " << cplex.getNcuts(IloCplex::CutUser) << std::endl;
         std::cout << "Time elapsed:                      " << cplex.getTime() << std::endl;
         std::cout << "Optimal value:                     " << cplex.getObjValue()-1 << std::endl;
         for (const auto &[name,value] : X)
            Xr[name] = cplex.getValue(value);
         for (const auto &v : g.nodes()) {
            if ( Xr[v.get()] == 1 ) {
               typename Node<2>::Queue queue;
               for (const auto &node : v->communication_queue())
                  if ( Xr[node] == 1 ) {
                     queue.push_front(node);
                  }
               v->set_new_sensor(queue);
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
