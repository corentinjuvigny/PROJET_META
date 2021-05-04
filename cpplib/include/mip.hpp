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

/** @file mip.hpp
 *
 * @brief Mixed integer algorithm to construct the optimal solution of the problem
 *
 */
#ifndef __MIP_HPP__
#define __MIP_HPP__

#include <grid.hpp>
#include <ilcplex/ilocplex.h>
#include <ios>
#include <sstream>
#include <unordered_map>

typedef std::map<std::string,IloNumVar> IloNumVarMap;

template <size_t d>
void mip_resolution(Grid<d> &g)
{
   IloEnv env;
   
   IloModel model(env);

   // Decision var
   IloNumVarMap X;
   for (const auto &s : g.nodes()) { 
      std::stringstream ss;
      ss << "x_" << s->name();
      X[s->name()] = IloNumVar(env,0,1,ILOBOOL,ss.str().c_str());
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
         e_com += X[v->name()];
      for (const auto &v : s->capture_queue())
         e_capt += X[v->name()];
      model.add(e_com >= 1);
      model.add(e_capt >= 1);
      e_capt.end();
      e_com.end();
   }
   model.add(X[g.well()->name()] == 1);

   IloCplex cplex(model);

   cplex.setParam(IloCplex::Param::Threads,8);
   cplex.setParam(IloCplex::Param::MIP::Strategy::File,3);
   cplex.solve();

   IloAlgorithm::Status status = cplex.getStatus();
   std::stringstream ss; 
   ss << "Solution status:                   " << cplex.getStatus();
   std::string separator_line(ss.str().length(),'-');
   std::cout << separator_line << std::endl;
   std::cout << ss.str() << std::endl;
   //IloNumMatrix xr(env,k);
   switch ( status ) {
      case IloAlgorithm::Optimal:
         std::cout << "Nodes processed:                   " << cplex.getNnodes() << std::endl;
         std::cout << "Active user cuts/lazy constraints: " << cplex.getNcuts(IloCplex::CutUser) << std::endl;
         std::cout << "Time elapsed:                      " << cplex.getTime() << std::endl;
         std::cout << "Optimal value:                     " << cplex.getObjValue() << std::endl;
        // for (IloInt p = 0; p < k; p++) {
        //    xr[p] = IloNumArray(env,m);
        //    cplex.getValues(xr[p],x[p]);
        // }
         break;
      default:
         break;
   }
   std::cout << separator_line << std::endl;


   model.end();
   cplex.end();
   env.end();
}

#endif //__MIP_HPP__
