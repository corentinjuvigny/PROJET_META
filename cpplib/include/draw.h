/*

Copyright (c) 2019-2020, Adrien BLASSIAU and Corentin JUVIGNY

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

/**
 * @file draw.h
 *
 * @brief Graphical drawing module to plot the data of our problem.
 *
 * The problem we want to solve is a 2-D graph problem so we can easily draw
 * it. For this, we use python because it's easier to use than GTK. Warnings,
 * it produces some memory leaks !
 *
 */

#ifndef __DRAW_H__
#define __DRAW_H__

#include "grid.hpp"

/**
 * Draw all the data of our problem is a 2D canvas generated with python
 *
 * @param g       All the data of our problem.
 * @param pause   The time we want to wait before closing the window.
 * @param size    The height of the plot.
 */
extern void draw_data_2D_Python(const Grid<2> &g, const float pause, const int size);


enum class DrawType { Python, OpenGL };

constexpr void draw_data_2D(const Grid<2> &g, const DrawType dt, const float pause, const int size)
{
   switch (dt) {
      case DrawType::Python:
         draw_data_2D_Python(g,pause,size);
         break;
      case DrawType::OpenGL:
         break;
      default:
         break;
   }
}

template <size_t d>
class GridDrawer {
   public:
      GridDrawer( const Grid<d> &g
              , const DrawType dt
              , const float pause
              , const int window_size)
         : _g(g), _dt(dt), _pause(pause), _window_size(window_size) { }
      void draw_data() const;
   private:
      const Grid<d> &_g;
      DrawType _dt;
      float _pause;
      int _window_size;
};

template <size_t d>
void GridDrawer<d>::draw_data() const
{
   std::cerr << "Drawing data for dim " << d << " not yet implemented" << std::endl;
}

template <>
inline void GridDrawer<2>::draw_data() const
{
   draw_data_2D(_g,_dt,_pause,_window_size);
}

template <size_t d>
void draw_data(const Grid<d> &g, const DrawType dt = DrawType::Python, const float pause = 10.0, const int size = 20)
{
   GridDrawer<d> drawer(g,dt,pause,size);
   drawer.draw_data();
}

#endif //__DRAW_H__
