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
#include "include.h"
#include "node.hpp"
#include "grid.hpp"
#include "rwfile.h"

int main(int argc, char* argv[])
{
   if ( argc < 2 ) {
      std::cout << "Use a filename as arg" << std::endl;
      return 0;
   }
#if 0
   const WellNode<2> node { "node1", {23.5,62.92} };
   std::cout << node << std::endl;
   Grid<2> mygrid { 20,4,2.5,4.2 };
   std::string s { " 2 5.123 7.2" };
   mygrid.insertNode(node_of_line(s,Node<2>::K_Well));
   std::cout << mygrid << std::endl;
#endif
   std::optional opt = read_node_file(argv[1],2.0,5.0); 
   std::cout << opt.value() << std::endl;
   opt->kdend();
   return 0;
}
