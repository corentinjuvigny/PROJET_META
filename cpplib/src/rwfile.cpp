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
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <cstring>
#include <cstdio>
#include "grid.hpp"

static size_t count_line(std::istream &is)
{
    // skip when bad
    if( is.bad() ) return 0;
    // save state
    std::istream::iostate state_backup = is.rdstate();
    // clear state
    is.clear();
    std::streampos pos_backup = is.tellg();

    is.seekg(0);
    size_t line_cnt;
    size_t lf_cnt = std::count(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>(), '\n');
    line_cnt = lf_cnt;
    // if the file is not end with '\n' , then line_cnt should plus 1
    is.unget();
    if( is.get() != '\n' ) { ++line_cnt ; }

    // recover state
    is.clear() ; // previous reading may set eofbit
    is.seekg(pos_backup);
    is.setstate(state_backup);

    return line_cnt;
}

static inline Node<2>::SNode node_of_line(const std::string &buf, const Node<2>::Kind kind)
{
      double x, y;
      char cpy[100];
      char name[50];
      std::fill(cpy,cpy+100,'\0');
      strncpy(cpy,buf.c_str(),buf.size());
      sscanf(cpy," %s %lf %lf",name,&x,&y);
      return std::make_shared<Node<2>>(Node<2>(kind,name,{x,y}));
}

std::optional<Grid<2>> read_node_file( const char* filename
                                     , const double communication_radius
                                     , const double capture_radius )
{
   std::ifstream file(filename);
   std::string line;
   if ( !file.is_open() ) {
      std::cerr << "File " << filename << " can't be opened" << std::endl;
      return std::nullopt;
   }
   size_t n = count_line(file);
   Grid<2> res { n, static_cast<long>(n-1), communication_radius, capture_radius };

   std::string buf;
   std::getline(file,buf);
   res.insertNode(node_of_line(buf,Node<2>::K_Well));
   while (std::getline(file,buf)) {
      res.insertNode(node_of_line(buf,Node<2>::K_Target));
   }

   file.close();

   res.finish();

   return std::make_optional(res);
}
