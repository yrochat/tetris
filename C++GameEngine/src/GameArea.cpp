/**
 * \brief
 *
 **/
/**
 \file GameArea.cpp
 \author steiner 
 \date Apr 13, 2016
 **/

#include "GameArea.h"

namespace TwitterTetris
{
// Check if a piece intersects the area
bool GameArea::intersect(const GamePiece& _piece) const
{
   bool res = false;

   std::matrix<bool>  piece_box = _piece.occupied_pixels();
   std::pair<int,int> piece_pos = _piece.pos();

   // Loop on piece_box dimensions
   for(int x=0; (x < piece_box.nbrows() && !res); ++x)
      for(int y=0; (y < piece_box.nbcols() && !res); ++y)
      {
         if( piece_box[x][y] && (this->areaUnit[piece_pos.first + x][piece_pos.second + y] != Tetrimino::NONE) )
         {
            res = true;
         }
      }

   return res;
}

// Freeze piece into area
void GameArea::freeze(const GamePiece& _piece)
{
   std::matrix<bool>  piece_box = _piece.occupied_pixels();
   std::pair<int,int> piece_pos = _piece.pos();

   // Loop on piece_box dimensions
   for(int x=0; x < piece_box.nbrows(); ++x)
      for(int y=0; y < piece_box.nbcols(); ++y)
      {
         if(piece_box[x][y])
            this->areaUnit[piece_pos.first + x][piece_pos.second + y] = _piece.tetrimino();
      }
}


int GameArea::fullrows()
{
   int res = 0;

   // Loop on rows from top to bottom
   for(int r = this->AREA_NBROWS-1; r >= 0; --r)
   {
      bool full = true;
      // Loop on columns to find empty place
      for(int c=0; c < this->AREA_NBCOLS; ++c)
      {
         if(this->areaUnit[r][c] == Tetrimino::NONE)
         {
            full = false; break;
         }
      }

      if(full) { this->erase_row(r); ++res; }
   }

   return res;
}

void GameArea::erase_row(int r)
{
   // Remove every tetrimino from current row - ensure that last rows is correctly erased
   for(int c=0; c < this->AREA_NBCOLS; ++c)
   {
      this->areaUnit[r][c] = Tetrimino::NONE;
   }

   // Move every row above this one down
   for(int row=r; row < this->AREA_NBROWS-1; ++row)
   {
      for(int c=0; c < this->AREA_NBCOLS; ++c)
      {
         this->areaUnit[row][c] = this->areaUnit[row+1][c];
         this->areaUnit[row+1][c] = Tetrimino::NONE;
      }
   }
}

} /* namespace TwitterTetris */
