/**
 * \brief Some routines for Cairo drawing.
 *
 **/
/**
 \file DrawingRoutines.h
 \author steiner 
 \date Apr 26, 2016
**/

#include "color.h"
#include <cairo/cairo.h>

#ifndef DRAWINGROUTINES_H_
#define DRAWINGROUTINES_H_

namespace TwT {

enum class align { LEFT, RIGHT, CENTER };

void cairo_set_source_rgb (cairo_t * cr, const std::color col);
void cairo_set_source_rgba (cairo_t * cr, const std::color col);

void draw_line(cairo_t * cr, const int x, const int y, const int x1, const int y1, const int width, const std::color col);
void draw_block(cairo_t * cr, const int posX, const int posY, const int Lx, const int Ly, const Tetrimino shape, const std::color col);
void draw_brick(cairo_t * cr, const int posX, const int posY, const int Lx, const int Ly, const float line_width, bool odd);
void rounded_rectangle(cairo_t * cr, const int x, const int y, const int width, const int height, const int line_width, const std::color stroke_col, const std::color fill_col, const double corner_radius_ratio = 0.1);
void box_and_text(cairo_t * cr, const int x, const int y, const int Lx, const int Ly, const int fontsize, const char* font_family, const bool bold, const std::string txt);
void text(cairo_t * cr, const int x, const int y, const int Lx, const int Ly, const int fontsize, const char* font_family, const bool bold, const std::string txt, const align text_align);

}


#endif /* DRAWINGROUTINES_H_ */
