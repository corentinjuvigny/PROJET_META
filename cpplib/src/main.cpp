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


#include <iostream>
#include <optional>
#include <memory>
#include "include.h"
#include "node.hpp"
#include "grid.hpp"
#include "greedy.hpp"
#include "rwfile.h"
#include "draw.h"

int main(int argc, char* argv[])
{
   if ( argc < 2 ) {
      std::cout << "Use a filename as arg" << std::endl;
      return 0;
   }
#if 0
   Node<2> node { Node<2>::K_Well, "node1", {23.5,62.92} };
   std::cout << node << std::endl;
   Grid<2> mygrid { 20,4,2.5,4.2 };
   std::string s { " 2 5.123 7.2" };
   mygrid.insertNode(node_of_line(s,Node<2>::K_Well));
   std::cout << mygrid << std::endl;

#else
   int size = 20;
   double g_time = 10.0;
   std::optional opt = read_node_file(argv[1],1.00001,1.00001); 
   if ( opt == std::nullopt ) {
      std::cout << "Error no grid has been generated" << std::endl;
      return 1;
   }
   std::cout << *opt << std::endl;
   greedy_construction(*opt);
   std::cout << "Greedy Algo succeeded : cover = " << opt->cover()
             << " with " << opt->solution().size() 
             << " elems :" << std::endl;
   for (auto elem : opt->solution())
      std::cout << elem.first << std::endl;
   //draw_data(*opt,g_time,size);
   opt->end();
#endif
   return 0;
}
