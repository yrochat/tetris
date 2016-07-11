/**
 * \brief Some routines for Cairo drawing.
 *
 **/
/**
 \file DrawingRoutines.cpp
 \author steiner 
 \date Apr 26, 2016
**/

#include "DrawingRoutines.h"

void TwT::cairo_set_source_rgb (cairo_t * cr, const std::color col)
{
   cairo_set_source_rgb(cr, col.r(), col.g(),col.b());
}

void TwT::cairo_set_source_rgba (cairo_t * cr, const std::color col)
{
   cairo_set_source_rgba(cr, col.r(), col.g(),col.b(), col.a());
}

// Draw a block at pos X,Y with dim Lx,Ly according to a given shape.
void TwT::draw_block(cairo_t * cr, const int posX, const int posY, const int Lx, const int Ly, const Tetrimino shape, const std::color col)
{
   const float border_width = 0.12;
   int  b = (std::min(Lx,Ly)*border_width);  if(b % 2 == 1) ++b;
   const int hb = b/2;

   // Base is always a black stroke around the block
   if(shape != Tetrimino::I)
   {
      cairo_set_source_rgb (cr, 0, 0, 0);
      cairo_set_line_width (cr, b);
      cairo_rectangle (cr, posX+hb, posY+hb, Lx-b, Ly-b);
      cairo_stroke (cr);
   }

   switch(shape)
   {
   case Tetrimino::I:
   {
      cairo_set_source_rgb (cr, 0.7, 0.7, 0.7);
      cairo_rectangle (cr, posX, posY, Lx, Ly);
      cairo_fill (cr);

      cairo_set_source_rgba (cr, 0.3, 0.3, 0.3, 0.5);
#if 1
      // Fill with repeated pattern
      std::vector<float> coordX { 0.5  , 0.2, 0.8 , 0.1 , 0.5  , 0.8 , 0.3 , 0.5 , 0.4 , 0.6 };
      std::vector<float> coordY { 0.1  , 0.2, 0.2 , 0.4 , 0.4  , 0.5 , 0.6 , 0.6 , 0.8 , 0.8 };
      float pLx = Lx/10.0; float pLy = Ly/10.0;
      for(size_t i=0; i < coordX.size(); ++i)
      {
         cairo_rectangle (cr, posX + Lx*coordX[i], posY + Ly*coordY[i], pLx , pLy);
         cairo_fill (cr);
      }
#else
      // Fill with N random pixels
      int N = 10; float pLx = Lx/10.0; float pLy = Ly/10.0;
      std::random_int_generator coordX(posX+pLx, posX+Lx-pLx);
      std::random_int_generator coordY(posY+pLy, posY+Ly-pLy);
      for(int i=1; i <= N; ++i)
      {
         cairo_rectangle (cr, coordX(), coordY(), pLx , pLy);
         cairo_fill (cr);
      }
#endif

      cairo_set_source_rgba (cr, 0, 0, 0, 0.2);
      cairo_set_line_width (cr, 2);
      cairo_rectangle (cr, posX+1, posY+1, Lx-2, Ly-2);
      cairo_stroke (cr);

      break;
   }
   case Tetrimino::L:
   {
      cairo_set_source_rgb (cr, 0.3,0.3,0.3);
      cairo_rectangle (cr, posX+b, posY+b, Lx-2*b, Ly-2*b);
      cairo_fill (cr);

      break;
   }
   case Tetrimino::J:
   {
      cairo_set_source_rgb (cr, 0.7, 0.7, 0.7);
      cairo_rectangle (cr, posX+b, posY+b, Lx-2*b, Ly-2*b);
      cairo_fill (cr);
      cairo_set_source_rgb (cr, 0,0,0);
      cairo_set_line_width (cr, b);
      cairo_rectangle (cr, posX+2*b+hb, posY+2*b+hb, Lx-5*b, Ly-5*b);
      cairo_stroke(cr);
      cairo_set_source_rgb (cr, 1,1,1);
      cairo_rectangle (cr, posX+3*b, posY+3*b, Lx-6*b, Ly-6*b);
      cairo_fill (cr);
      break;
   }
   case Tetrimino::O:
   {
      cairo_set_source_rgb (cr, 0, 0, 0);
      cairo_set_line_width (cr, b);
      cairo_rectangle (cr, posX+b+hb, posY+b+hb, Lx-3*b, Ly-3*b);
      cairo_fill_preserve (cr);
      cairo_set_source_rgb (cr, 1,1,1);
      cairo_stroke (cr);

      break;
   }
   case Tetrimino::Z:
   {
      cairo_set_source_rgb (cr, 0.7, 0.7, 0.7);
      cairo_rectangle (cr, posX+b, posY+b, Lx-2*b, Ly-2*b);
      cairo_fill (cr);
      cairo_set_source_rgb (cr, 0,0,0);
      cairo_rectangle (cr, posX+(Lx-2*b)/2, posY+(Ly-2*b)/2, 2*b, 2*b);
      cairo_fill (cr);
      break;
   }
   case Tetrimino::T:
   {
      cairo_set_source_rgb (cr, 0.7, 0.7, 0.7);
      cairo_rectangle (cr, posX+b, posY+b, Lx-2*b, Ly-2*b);
      cairo_fill (cr);
      cairo_set_source_rgb (cr, 0, 0, 0);
      cairo_move_to(cr, posX+2*b, posY+Ly-2*b);
      cairo_line_to(cr, posX+Lx-2*b, posY+Ly-2*b);
      cairo_line_to(cr, posX+Lx-2*b, posY+2*b);
      cairo_stroke(cr);
      cairo_set_source_rgb (cr, 1, 1, 1);
      cairo_move_to(cr, posX+Lx-2*b, posY+2*b);
      cairo_line_to(cr, posX+2*b, posY+2*b);
      cairo_line_to(cr, posX+2*b, posY+Ly-2*b);
      cairo_stroke(cr);

      break;
   }
   case Tetrimino::S:
   {
      cairo_set_source_rgb (cr, 0.3, 0.3, 0.3);
      cairo_rectangle (cr, posX+b, posY+b, Lx-2*b, Ly-2*b);
      cairo_fill (cr);
      cairo_set_source_rgb (cr, 0,0,0);
      cairo_set_line_width (cr, b);
      cairo_rectangle (cr, posX+2*b+hb, posY+2*b+hb, Lx-5*b, Ly-5*b);
      cairo_stroke(cr);
      cairo_set_source_rgb (cr, 1,1,1);
      cairo_rectangle (cr, posX+3*b, posY+3*b, Lx-6*b, Ly-6*b);
      cairo_fill (cr);
      break;
   }
   default:
   {
      cairo_set_source_rgb (cr, col.r(), col.g(), col.b());
      cairo_fill (cr);
   }
   }
}

void TwT::draw_brick(cairo_t * cr, const int posX, const int posY, const int Lx, const int Ly, const float line_width, bool odd){

   cairo_set_source_rgb (cr, 0.5, 0.5, 0.5);
   cairo_rectangle (cr, posX, posY, Lx, Ly);
   cairo_fill(cr);

   float h = (Ly)/3.0;

   // Horiz lines
   draw_line(cr,posX, posY-0.5*line_width,     posX+Lx, posY-0.5*line_width,     line_width,   std::color(0,0,0));
   draw_line(cr,posX, posY+h-0.5*line_width,   posX+Lx, posY+h-0.5*line_width,   line_width,   std::color(0,0,0));
   draw_line(cr,posX, posY+2*h-0.5*line_width, posX+Lx, posY+2*h-0.5*line_width, line_width,   std::color(0,0,0));
   draw_line(cr,posX, posY+Ly-0.5*line_width,  posX+Lx, posY+Ly-0.5*line_width,  line_width,   std::color(0,0,0));

   // Vertical lines and "light effect"
   float hx3 = (Lx)/4.0;
   float hx2 = (Lx)/2.0;

   if(odd)
   {
      draw_line(cr,posX+hx3-0.5*line_width,   posY, posX+hx3-0.5*line_width,     posY+h,   line_width, std::color(0,0,0));
      draw_line(cr,posX+hx3+hx2-0.5*line_width, posY, posX+hx3+hx2-0.5*line_width,   posY+h,   line_width, std::color(0,0,0));

      draw_line(cr,posX+hx2-0.5*line_width,   posY+h, posX+hx2-0.5*line_width,   posY+2*h, line_width, std::color(0,0,0));
      draw_line(cr,posX+2*hx2-0.5*line_width, posY+h, posX+2*hx2-0.5*line_width, posY+2*h, line_width, std::color(0,0,0));

      draw_line(cr,posX+hx3-0.5*line_width,   posY+2*h, posX+hx3-0.5*line_width,     posY+Ly,   line_width, std::color(0,0,0));
      draw_line(cr,posX+hx3+hx2-0.5*line_width, posY+2*h, posX+hx3+hx2-0.5*line_width,   posY+Ly,   line_width, std::color(0,0,0));
   }
   else
   {
      draw_line(cr,posX+hx2-0.5*line_width,   posY, posX+hx2-0.5*line_width,     posY+h,   line_width, std::color(0,0,0));
      draw_line(cr,posX+2*hx2-0.5*line_width, posY, posX+2*hx2-0.5*line_width,   posY+h,   line_width, std::color(0,0,0));

      draw_line(cr,posX+hx3-0.5*line_width,   posY+h, posX+hx3-0.5*line_width,   posY+2*h, line_width, std::color(0,0,0));
      draw_line(cr,posX+hx3+hx2-0.5*line_width, posY+h, posX+hx3+hx2-0.5*line_width, posY+2*h, line_width, std::color(0,0,0));

      draw_line(cr,posX+hx2-0.5*line_width,   posY+2*h, posX+hx2-0.5*line_width,     posY+Ly,   line_width, std::color(0,0,0));
      draw_line(cr,posX+2*hx2-0.5*line_width, posY+2*h, posX+2*hx2-0.5*line_width,   posY+Ly,   line_width, std::color(0,0,0));

   }
}

// PI
double pi() { return 4.0*atan(1.0); }

void TwT::rounded_rectangle(cairo_t * cr, const int x, const int y, const int width, const int height, const int line_width, const std::color stroke_col, const std::color fill_col, const double corner_radius_ratio)
{
   double radius = corner_radius_ratio*std::min(width, height);   /* and corner curvature radius */
   double degrees = pi() / 180.0;

   cairo_new_sub_path (cr);
   cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
   cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
   cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
   cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
   cairo_close_path (cr);

   cairo_set_source_rgb (cr, fill_col.r(), fill_col.g(), fill_col.b());
   cairo_fill (cr);

   const float offset = 1.5*line_width;

   cairo_new_sub_path (cr);
   cairo_arc (cr, x + width - offset- radius, y + offset + radius, radius, -90 * degrees, 0 * degrees);
   cairo_arc (cr, x + width - offset- radius, y + height - offset - radius, radius, 0 * degrees, 90 * degrees);
   cairo_arc (cr, x + offset + radius, y + height - offset - radius, radius, 90 * degrees, 180 * degrees);
   cairo_arc (cr, x + offset + radius, y + offset + radius, radius, 180 * degrees, 270 * degrees);
   cairo_close_path (cr);
   cairo_set_source_rgb (cr, stroke_col.r(), stroke_col.g(), stroke_col.b());
   cairo_set_line_width (cr, line_width);
   cairo_stroke (cr);
}

void TwT::draw_line(cairo_t * cr, const int x, const int y, const int x1, const int y1, const int width, const std::color col)
{
   cairo_set_source_rgba (cr, col);
   cairo_move_to (cr, x, y );
   cairo_line_to(cr, x1, y1);
   cairo_set_line_width (cr, width);
   cairo_stroke (cr);
}

void TwT::box_and_text(cairo_t * cr, const int x, const int y, const int Lx, const int Ly, const int fontsize, const char* font_family, const bool bold, const std::string txt)
{
   rounded_rectangle(cr, x, y, Lx, Ly, 3, std::grey5, std::white, 0.05);

   cairo_text_extents_t extents;

   cairo_select_font_face (cr, font_family, CAIRO_FONT_SLANT_NORMAL, bold?CAIRO_FONT_WEIGHT_BOLD:CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size (cr, fontsize);
   cairo_text_extents (cr, txt.c_str(), &extents);

   cairo_set_source_rgb (cr, std::black);
   cairo_move_to (cr, x + 0.5*(Lx-extents.width), y + Ly - 0.5*(Ly-extents.height));
   cairo_show_text (cr, txt.c_str());

}


void TwT::text(cairo_t * cr, const int x, const int y, const int Lx, const int Ly, const int fontsize, const char* font_family, const bool bold, const std::string txt, const align text_align)
{
   cairo_text_extents_t extents;

   cairo_select_font_face (cr, font_family, CAIRO_FONT_SLANT_NORMAL, bold?CAIRO_FONT_WEIGHT_BOLD:CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size (cr, fontsize);
   cairo_text_extents (cr, txt.c_str(), &extents);

   cairo_set_source_rgb (cr, std::black);
   if(text_align == align::CENTER)
   {
      cairo_move_to (cr, x + 0.5*(Lx-extents.width), y + Ly - 0.5*(Ly-extents.height));
   }
   else if(text_align == align::LEFT)
   {
      cairo_move_to (cr, x, y + Ly - 0.5*(Ly-extents.height));
   }
   else // align right
   {
      cairo_move_to (cr, x + (Lx-extents.width), y + Ly - 0.5*(Ly-extents.height));
   }
   cairo_show_text (cr, txt.c_str());

}
