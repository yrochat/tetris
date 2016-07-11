/**
 * \brief The GameState makes the game. Store and modify the game state, piece position, score, etc.
 *
 **/
/**
 \file GameState.cpp
 \author steiner 
 \date Apr 13, 2016
 **/

#include "GameState.h"
#include "DrawingRoutines.h"

namespace TwT
{
// Generate a state for a new game
void GameState::NewGame()
{
   // Game is not over
   this->isGameOver = false;

   // Resize and erase draw area
   this->draw_area = GameArea(this->properties.GameAreaNbRows(), this->properties.GameAreaNbCols());

   // Randomly choose a piece to begin with
   current_piece = GamePiece::random_piece();
   next_piece = GamePiece::random_piece();

   // Place current_piece in initial position
   this->enter_piece(current_piece);
}

// Read an XML archive if available or generate a random piece if not
GameState::GameState(const std::string filename, const GameProperties& _properties)
{
   std::ifstream file(filename.c_str());
   if(file.good())
   {
      cereal::XMLInputArchive archive(file);
      GameState TMP; archive(TMP); *this = TMP;
   }
   else
   {
      std::cout << "File " << filename << " is not readable for XML deserialization. Creating a new game." << std::endl;
      this->NewGame();
   }

   // Compare old and new properties for consistency
   if(this->properties.GameAreaNbRows() != _properties.GameAreaNbRows() || this->properties.GameAreaNbCols() != _properties.GameAreaNbCols())
   {
      std::cout << "Properties are not coherent. Updating them and starting a new game." << std::endl;
      this->properties = _properties;
      this->NewGame();
   }
}

// Apply an action on a given state to create a new state
GameState::GameState(const GameState& from_state, const GameAction& action)
{
   if( from_state.isGameOver )
   {
      // If from_state was game_over, restart a new game and ignore action
      this->NewGame();
   }
   else
   {
      // Start from last state
      *this = from_state;

      // For each action do something
      GamePiece tmp = this->current_piece;

      bool check_or_revert = false;
      switch(action())
      {
      case Action::LEFT : // Try to go one unit left
      {
         tmp.addto_position(0, -1); check_or_revert = true;
         break;
      }
      case Action::RIGHT : // Try to go one unit right
      {
         tmp.addto_position(0, +1); check_or_revert = true;
         break;
      }
      case Action::DOWN :
      {
         tmp.addto_position(-1, 0); // Try to go one unit down
         if(this->draw_area.isin(tmp)) {
            if( this->draw_area.intersect(tmp) )  {
               // revert and freeze piece
               this->draw_area.freeze(this->current_piece); this->new_piece();
            }
            else { this->current_piece = tmp; }
         }
         else {
            this->draw_area.freeze(this->current_piece); this->new_piece();
         }
         break;
      }
      case Action::FALL :
      {
         while(true)
         {
            tmp.addto_position(-1, 0); // Try to go one unit down
            if(this->draw_area.isin(tmp)) {
               if( this->draw_area.intersect(tmp) )  {
                  // revert and freeze piece
                  this->draw_area.freeze(this->current_piece); this->new_piece(); break;
               }
               else { this->current_piece = tmp; }
            }
            else {
               this->draw_area.freeze(this->current_piece); this->new_piece(); break;
            }
         }

         break;
      }
      case Action::SPIN :
      {
         tmp.spin(); // Spin piece

         if(this->draw_area.isout(tmp) || this->draw_area.intersect(tmp) ) // If spinned piece is out or intersects, try moving it 1 unit left or right
         {
            // Move left
            tmp.addto_position(0, -1);

            // Recheck
            if(this->draw_area.isout(tmp) || this->draw_area.intersect(tmp) )
            {
               // Move right
               tmp.addto_position(0, +2);

               // Recheck
               if(this->draw_area.isout(tmp) || this->draw_area.intersect(tmp) )
               {
                  // Move down
                  tmp.addto_position(-1, -1);

                  if(!(this->draw_area.isout(tmp) || this->draw_area.intersect(tmp) ))
                  {
                     this->current_piece = tmp; // spin is successfull with a down move
                  }
               } else { this->current_piece = tmp; } // spin is successfull with a right move
            } else { this->current_piece = tmp; } // spin is successfull with a left move
         }
         else { this->current_piece = tmp; } // spin is successfull
         break;
      }
      case Action::ANTISPIN :
      {
         tmp.antispin(); // Spin piece

         if(this->draw_area.isout(tmp) || this->draw_area.intersect(tmp) ) // If spinned piece is out or intersects, try moving it 1 unit left or right
         {
            // Move left
            tmp.addto_position(0, -1);

            // Recheck
            if(this->draw_area.isout(tmp) || this->draw_area.intersect(tmp) )
            {
               // Move right
               tmp.addto_position(0, +2);

               // Recheck
               if(this->draw_area.isout(tmp) || this->draw_area.intersect(tmp) )
               {
                  // Move down
                  tmp.addto_position(-1, -1);

                  if(!(this->draw_area.isout(tmp) || this->draw_area.intersect(tmp) ))
                  {
                     this->current_piece = tmp; // spin is successfull with a down move
                  }
               } else { this->current_piece = tmp; } // spin is successfull with a right move
            } else { this->current_piece = tmp; } // spin is successfull with a left move
         }
         else { this->current_piece = tmp; } // spin is successfull
         break;
      }
      default : // Do nothing
         break;
      }

      // Check that new position is admissible and eventually revert action
      if(check_or_revert) {
         if( this->draw_area.isin(tmp) ) {
            if( !this->draw_area.intersect(tmp) )
               this->current_piece = tmp; /* Revert action */
         }
      }

      // Check full rows
      int full = this->draw_area.fullrows();

      // Increase score, lines and level
      this->score += (this->level+1)*this->properties.Points(full);
      this->lines += full;

      if(this->score % (this->level*10000) == 0 && this->score != 0)
         ++this->level;
   }
}

// XML serialization of GameState
void GameState::export_xml(const std::string filename) const
{
   std::ofstream file;
   if(filename.size() > 0)
      file.open(filename.c_str());
   else
      file.open("last_state.xml");

   if(file.good())
   {
      cereal::XMLOutputArchive archive(file);
      GameState TMP = *this;
      archive(TMP);
   }
   else
   {
      throw std::bad_file_access(filename);
   }
}


// Place a piece in an available initial position or declare game_over
void GameState::enter_piece(GamePiece& _piece)
{
   std::pair<int,int> bb = _piece.boundingbox();
   _piece.set_position( this->draw_area.nbrows()-bb.first , (this->draw_area.nbcols()-bb.second)/2);

   // Check intersection between area and _piece
   if( this->draw_area.intersect( _piece)  ) this->isGameOver = true;
}

// Export the state in the console
void GameState::export_ascii_stream(std::ostream & sout) const
{
   // Generate a matrix with the characters to be exported
   std::matrix<char> out(this->draw_area.nbrows(), this->draw_area.nbcols());

   // Fill the out structure with the unit already on the draw area
   int outrow = 0;
   for(int r = this->draw_area.nbrows()-1; r >= 0; --r, ++outrow) // In the draw area we go from bottom to top. In the ascii output we have to do the other way.
   {
      for(int c = 0; c < this->draw_area.nbcols(); ++c)
      {
         out[outrow][c] = this->properties.PieceChar( this->draw_area(r,c) );
      }
   }

   // Add current piece
   std::matrix<bool> _piecemat = this->current_piece.occupied_pixels();
   char tt = this->properties.PieceChar( this->current_piece.tetrimino() );
   auto pos = this->current_piece.pos();

   for(int r=0; r < _piecemat.nbrows(); ++r)
      for(int c=0; c < _piecemat.nbcols(); ++c)
      {
         if(_piecemat[r][c])
         {
            out[this->draw_area.nbrows()-1-(pos.first + r)][pos.second + c] = tt;
         }
      }

   // next piece
   std::matrix<bool> _nextpiecemat = this->next_piece.occupied_pixels();
   char nexttt = this->properties.PieceChar( this->next_piece.tetrimino() );

   std::matrix<char> _displaynext(6,6, ' ');
   for(int c=0; c < 6; ++c) {
      _displaynext[0][c] = '-'; _displaynext[5][c] = '-';
   }
   for(int r=1; r < 5; ++r) {
         _displaynext[r][0] = '|'; _displaynext[r][5] = '|';
   }

   for(int r=0; r < _nextpiecemat.nbrows(); ++r)
        for(int c=0; c < _nextpiecemat.nbcols(); ++c)
        {
           if(_nextpiecemat[r][c])
              _displaynext[4-r][c+1] = nexttt;
        }

   // Display output in terminal
   sout << std::endl
        << std::setw(out.nbcols() + 2) << std::setfill('-') << '-' << std::setfill(' ') << std::endl;

   for(int r=0; r < out.nbrows(); ++r)
   {
      sout << '|';
      for(int c = 0; c < out.nbcols(); ++c)
         sout << out[r][c];
      sout << '|';

      // Check for Game Over
      if(this->isGameOver && r == out.nbrows()-11) { sout << " !! GAME OVER !!"; }

      if(r >= out.nbrows()-10 && r < out.nbrows()-4)
      {
         sout << " ";
         for(int c = 0; c < _displaynext.nbcols(); ++c)
            sout << _displaynext[r-(out.nbrows()-10)][c];
      }

      if(r == out.nbrows()-3) { sout << " Level " << this->level; }
      if(r == out.nbrows()-2) { sout << " Score " << this->score; }
      if(r == out.nbrows()-1) { sout << " Lines " << this->lines; }

      sout << std::endl;
   }
   sout << std::setw(out.nbcols() + 2) << std::setfill('-') << '-' << std::setfill(' ') << std::endl;
}


// Export the state to an image showing the situation
void GameState::export_png(const std::string filename) const
{
   // ----------------------------------------------------------
   // Compute image dimensions
   // ----------------------------------------------------------
   // Unit is the reference dimension. It is the dimension of a
   // Tetris block (each Tetrimino is build with 4 blocks)

   // How many pixel per unit block ? This will give the final
   // resolution of the image
   const int pixel_per_unit = 30; // This should be even

   // Spaces around drawing in unit
   const float space_up = 0.05, space_down = 0.0, space_left = 0.15, space_right = 0.5, wall_width = 1;

   // Horizontal space between game area and game informations
   const float space_center = 0.5;

   // Vertical space between next block area and text information
   const float space_up_to_infos = 2;

   // Compute horizontal and vertical dimensions
   float horiz = 0, vert = 0;
   horiz += space_left + this->draw_area.nbcols() + space_center + 5 + space_right + 2*wall_width; // 5 is the fixed size of the box displaying the next piece
   vert  += space_down + this->draw_area.nbrows() + space_up; // We assume that drawing area is always bigger than the right info area

   // Dimensions of the global image in pixels
   const int Total_Lx = horiz*pixel_per_unit;
   const int Total_Ly = vert*pixel_per_unit;

   // ----------------------------------------------------------
   // Usefull cairo variable
   // ----------------------------------------------------------
   cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, Total_Lx, Total_Ly);
   cairo_t *cr = cairo_create (surface);

   // Font size for text
   int fsize = std::round(1.1*pixel_per_unit); if(fsize % 2 != 0) fsize += 1;
   const float offset = 4;

   // ----------------------------------------------------------
   // Fill entire image with a background color
   // ----------------------------------------------------------
   cairo_set_source_rgb (cr, 0.92, 0.92, 0.92);
   cairo_rectangle (cr, 0, 0, Total_Lx, Total_Ly);
   cairo_fill (cr);

   // ----------------------------------------------------------
   // Make left wall
   // ----------------------------------------------------------
   const int WallLeftTopLeftX = (space_left)*pixel_per_unit;
   const int WallLeftTopLeftY = space_up*pixel_per_unit;
   const int WallLX = wall_width*pixel_per_unit+3;
   const float brick_line_width = 4;

   // For now we just draw a filled wall
   for(int r=0; r < this->draw_area.nbrows(); ++r)
   {
      int posY = WallLeftTopLeftX + (this->draw_area.nbrows()-1-r)*pixel_per_unit;
      draw_brick(cr, WallLeftTopLeftY, posY, WallLX, pixel_per_unit, brick_line_width, (r % 2 != 0));
   }

   // ----------------------------------------------------------
   // Construct the playing area
   // ----------------------------------------------------------
   const int DrawingAreaTopLeftX =  (space_left+wall_width)*pixel_per_unit;
   const int DrawingAreaTopLeftY =  space_up*pixel_per_unit;
   const int DrawingAreaLX =  this->draw_area.nbcols()*pixel_per_unit;
   const int DrawingAreaLY =  this->draw_area.nbrows()*pixel_per_unit;

   // Fill every odd column with a particular color
   std::color odd_bg = std::grey7; double odd_bg_alpha = 0.2;
   cairo_set_source_rgba (cr, odd_bg.r(), odd_bg.g(), odd_bg.b(), odd_bg_alpha);
   for(int c=1; c < this->draw_area.nbcols(); c = c+2)
   {
      cairo_rectangle (cr, DrawingAreaTopLeftX + c*pixel_per_unit, DrawingAreaTopLeftY, pixel_per_unit, DrawingAreaLY);
      cairo_fill (cr);
   }

   // ----------------------------------------------------------
   // Fill area with existing freezed pieces
   // ----------------------------------------------------------
   for(int r=0; r < this->draw_area.nbrows(); ++r)
      for(int c=0; c < this->draw_area.nbcols(); ++c)
      {
         if(this->draw_area(r, c) != Tetrimino::NONE)
         {
            std::color col = this->properties.PieceColor( this->draw_area(r, c) );

            int posX = DrawingAreaTopLeftX + c*pixel_per_unit;
            int posY = DrawingAreaTopLeftY + (this->draw_area.nbrows()-1-r)*pixel_per_unit;

            draw_block(cr, posX, posY, pixel_per_unit, pixel_per_unit, this->draw_area(r, c), col);
         }
      }

   // ----------------------------------------------------------
   // Add current piece falling
   // ----------------------------------------------------------
   std::matrix<bool> current_occupied = current_piece.occupied_pixels();

   for(int r=0; r < current_occupied.nbrows(); ++r)
         for(int c=0; c < current_occupied.nbcols(); ++c)
         {
            if(current_occupied[r][c])
            {
               int posX = DrawingAreaTopLeftX + (c + current_piece.pos().second)*pixel_per_unit;
               int posY = DrawingAreaTopLeftY + (this->draw_area.nbrows()-1-current_piece.pos().first-r)*pixel_per_unit;

               std::color col = this->properties.PieceColor( current_piece.tetrimino() );

               draw_block(cr, posX, posY, pixel_per_unit, pixel_per_unit, current_piece.tetrimino(), col);

            }
         }

   // ----------------------------------------------------------
   // Make right wall
   // ----------------------------------------------------------
   const int WallRightTopLeftX = (DrawingAreaTopLeftX+DrawingAreaLX);
   const int WallRightTopLeftY = space_up*pixel_per_unit;

   // For now we just draw a filled wall
   for(int r=0; r < this->draw_area.nbrows(); ++r)
   {
      int posY = WallRightTopLeftY + (this->draw_area.nbrows()-1-r)*pixel_per_unit;
      draw_brick(cr, WallRightTopLeftX, posY, WallLX, pixel_per_unit, brick_line_width, (r % 2 != 0));
   }

   // ----------------------------------------------------------
   // Draw a black background
   // ----------------------------------------------------------
   float SCORE_TY = 0;
   {
      int TopLeftX = (space_left + this->draw_area.nbcols() + 2*wall_width)*pixel_per_unit + offset;
      int TopLeftY = 0;
      float LX =  Total_Lx-TopLeftX;
      float LY =  Total_Ly;
      cairo_set_source_rgb (cr, std::black);
      cairo_rectangle (cr, TopLeftX, TopLeftY, LX, LY);
      cairo_fill (cr);

      TopLeftY = (space_up+space_up_to_infos)*pixel_per_unit + 0.9*pixel_per_unit;
      draw_line(cr, TopLeftX, TopLeftY, Total_Lx, TopLeftY, offset, std::white);

      float LAST_LY = offset; LY = 0.8*pixel_per_unit;
      TopLeftY += (LAST_LY + LY)/2;
      draw_line(cr, TopLeftX, TopLeftY, Total_Lx, TopLeftY, LY, std::grey5);

      LAST_LY = LY; LY = offset;
      TopLeftY += (LAST_LY + LY)/2;
      draw_line(cr, TopLeftX, TopLeftY, Total_Lx, TopLeftY, LY, std::white);

      LAST_LY = LY; LY = offset;
      TopLeftY += (LAST_LY + LY)/2;
      draw_line(cr, TopLeftX, TopLeftY, Total_Lx, TopLeftY, LY, std::grey5);

      SCORE_TY = TopLeftY;
      LAST_LY = LY; LY = 1.4*pixel_per_unit;
      TopLeftY += (LAST_LY + LY)/2;
      draw_line(cr, TopLeftX, TopLeftY, Total_Lx, TopLeftY, LY, std::white);

      LAST_LY = LY; LY = offset;
      TopLeftY += (LAST_LY + LY)/2;
      draw_line(cr, TopLeftX, TopLeftY, Total_Lx, TopLeftY, LY, std::grey5);

      LAST_LY = LY; LY = offset;
      TopLeftY += (LAST_LY + LY)/2;
      draw_line(cr, TopLeftX, TopLeftY, Total_Lx, TopLeftY, LY, std::white);

   }
   // ----------------------------------------------------------
   // Draw a score box
   // ----------------------------------------------------------
   {
      const int TopLeftX = (space_left + this->draw_area.nbcols() + space_center + 2*wall_width)*pixel_per_unit;
      const int TopLeftY = (space_up+space_up_to_infos)*pixel_per_unit;
      const int LX =  5*pixel_per_unit;
      const int LY =  1.5*pixel_per_unit;

      std::string txt = "SCORE";

      // Other fonts that I have tried
      // box_and_text(cr, TopLeftX, TopLeftY, LX, LY, 1.7*pixel_per_unit, "Edit Undo BRK", false, txt);
      // box_and_text(cr, TopLeftX, TopLeftY+ 2.05*LY, LX, LY, 1.0*pixel_per_unit, "Lilliput Steps", false, txt);
      // box_and_text(cr, TopLeftX, TopLeftY+ 3.05*LY, LX, LY, 1.7*pixel_per_unit, "Orange Kid", true, txt);
      // box_and_text(cr, TopLeftX, TopLeftY+ 4.05*LY, LX, LY, 1.5*pixel_per_unit, "Unifont", true, txt);

      // Draw a box for score label
      box_and_text(cr, TopLeftX, TopLeftY, LX, LY, fsize, "Joystix", true, txt);

      // Write score
      txt = std::to_string(this->score);
      text(cr, TopLeftX, SCORE_TY, LX, LY, fsize, "Joystix", true, txt, align::RIGHT);
   }

   // ----------------------------------------------------------
   // Draw a level box
   // ----------------------------------------------------------
   {
        float TopLeftX = (space_left + this->draw_area.nbcols() + space_center + 2*wall_width)*pixel_per_unit;
        float TopLeftY = (space_up+space_up_to_infos)*pixel_per_unit + 5*pixel_per_unit;
        float LX =  5*pixel_per_unit;
        float LY =  2.6*pixel_per_unit;
        rounded_rectangle(cr, TopLeftX, TopLeftY, LX, LY, 3, std::grey5, std::white, 0.05);

        LY = 1.4*pixel_per_unit;

        // Write level label
        std::string txt = "LEVEL";
        text(cr, TopLeftX, TopLeftY, LX, LY, fsize, "Joystix", true, txt, align::CENTER);

        // Write level
        txt = std::to_string(this->level);
        text(cr, TopLeftX, TopLeftY + fsize, LX, LY, fsize, "Joystix", true, txt, align::CENTER);
   }

   // ----------------------------------------------------------
   // Draw a lines box
   // ----------------------------------------------------------
   {
        float TopLeftX = (space_left + this->draw_area.nbcols() + space_center + 2*wall_width)*pixel_per_unit;
        float TopLeftY = (space_up+space_up_to_infos)*pixel_per_unit + 8*pixel_per_unit + 2*offset;
        float LX =  5*pixel_per_unit;
        float LY =  2.6*pixel_per_unit;
        rounded_rectangle(cr, TopLeftX, TopLeftY, LX, LY, 3, std::grey5, std::white, 0.05);

        LY = 1.4*pixel_per_unit;

        // Write level label
        std::string txt = "LINES";
        text(cr, TopLeftX, TopLeftY, LX, LY, fsize, "Joystix", true, txt, align::CENTER);

        // Write level
        txt = std::to_string(this->lines);
        text(cr, TopLeftX, TopLeftY + fsize, LX, LY, fsize, "Joystix", true, txt, align::CENTER);
   }

   // ----------------------------------------------------------
   // Construct a box for next piece
   // ----------------------------------------------------------
   const int NextAreaTopLeftX = (space_left + this->draw_area.nbcols() + space_center + 2*wall_width)*pixel_per_unit;
   const int NextAreaTopLeftY = (space_up+space_up_to_infos)*pixel_per_unit + 12*pixel_per_unit + 2*offset;
   const int NextAreaLX =  5*pixel_per_unit;
   const int NextAreaLY =  4*pixel_per_unit;

   // Display title for the next box
   rounded_rectangle(cr, NextAreaTopLeftX, NextAreaTopLeftY, NextAreaLX, NextAreaLY, 3, std::grey5, std::white, 0.05);

   // Add next piece in box
   std::matrix<bool> next_occupied = next_piece.occupied_pixels();
   for(int r=0; r < next_occupied.nbrows(); ++r)
         for(int c=0; c < next_occupied.nbcols(); ++c)
         {
            if(next_occupied[r][c])
            {
               int posX = NextAreaTopLeftX + (c + ( (5-next_piece.boundingbox().second) / 2 ) )*pixel_per_unit;
               int posY = NextAreaTopLeftY + (3-(r + (4-next_piece.boundingbox().first) / 2 ))*pixel_per_unit;

               std::color col = this->properties.PieceColor( next_piece.tetrimino() );

               draw_block(cr, posX, posY, pixel_per_unit, pixel_per_unit, next_piece.tetrimino(), col);
            }
         }

   // ----------------------------------------------------------
   // Free memory for cairo objects
   // ----------------------------------------------------------
   cairo_destroy (cr);
   cairo_surface_write_to_png (surface, filename.c_str());
   cairo_surface_destroy (surface);
}


} /* namespace TwT */
