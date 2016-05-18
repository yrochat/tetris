/**
 * \brief
 *
 **/
/**
 \file GamePiece.cpp
 \author steiner 
 \date Apr 13, 2016
 **/

#include "GamePiece.h"

namespace TwitterTetris
{
// Compute bounding box of a piece according to its position, form and orientation
std::pair<int,int> GamePiece::boundingbox() const
{
   // Any piece would at least fit this bounding box
   std::pair<int,int> res { 4, 4 }; // LX, LY from lower left to upper right

   switch(this->form)
   {
   case Tetrimino::I :
   {
      if(this->ori == Orientation::LEFT || this->ori == Orientation::RIGHT) {
         res = std::make_pair(1, 4);
      }
      else {
         res = std::make_pair(4, 1);
      }
      break;
   }
   case Tetrimino::O :
   {
      res = std::make_pair(2, 2);
      break;
   }
   default:
   {
      if(this->ori == Orientation::LEFT || this->ori == Orientation::RIGHT) {
         res = std::make_pair(3, 2);
      }
      else {
         res = std::make_pair(2, 3);
      }
   }
   }

   return res;
}

// Return the matrix of the occupied space of the piece in the bounding box
std::matrix<bool> GamePiece::occupied_pixels() const
{
   std::pair<int,int> bb = this->boundingbox();
   std::matrix<bool> res(bb.first, bb.second, true); // Fill all bb with full pixels

   // Set occupied pixels for each form and orientation
   switch(this->form)
   {
   case Tetrimino::J :
   {
      switch(this->ori)
      {
      case Orientation::UP:    { res[0][0] = false; res[0][1] = false; break;}
      case Orientation::LEFT:  { res[0][1] = false; res[1][1] = false; break;}
      case Orientation::DOWN:  { res[1][1] = false; res[1][2] = false; break;}
      case Orientation::RIGHT: { res[1][0] = false; res[2][0] = false; break;}
      }
      break;
   }
   case Tetrimino::L :
   {
      switch(this->ori)
      {
      case Orientation::UP:    { res[0][1] = false; res[0][2] = false; break;}
      case Orientation::LEFT:  { res[1][1] = false; res[2][1] = false; break;}
      case Orientation::DOWN:  { res[1][0] = false; res[1][1] = false; break;}
      case Orientation::RIGHT: { res[0][0] = false; res[1][0] = false; break;}
      }
      break;
   }
   case Tetrimino::S :
   {
      switch(this->ori)
      {
      case Orientation::UP:    { res[1][0] = false; res[0][2] = false; break;}
      case Orientation::LEFT:  { res[0][0] = false; res[2][1] = false; break;}
      case Orientation::DOWN:  { res[1][0] = false; res[0][0] = false; break;}
      case Orientation::RIGHT: { res[0][0] = false; res[2][1] = false; break;}
      }
      break;
   }
   case Tetrimino::Z :
   {
      switch(this->ori)
      {
      case Orientation::UP:    { res[0][0] = false; res[1][2] = false; break;}
      case Orientation::LEFT:  { res[0][1] = false; res[2][0] = false; break;}
      case Orientation::DOWN:  { res[0][0] = false; res[1][2] = false; break;}
      case Orientation::RIGHT: { res[0][1] = false; res[2][0] = false; break;}
      }
      break;
   }
   case Tetrimino::T :
   {
      switch(this->ori)
      {
      case Orientation::UP:    { res[0][0] = false; res[0][2] = false; break;}
      case Orientation::LEFT:  { res[0][1] = false; res[2][1] = false; break;}
      case Orientation::DOWN:  { res[1][0] = false; res[1][2] = false; break;}
      case Orientation::RIGHT: { res[0][0] = false; res[2][0] = false; break;}
      }
      break;
   }
   case Tetrimino::NONE :
   {
      res = std::matrix<bool>(bb.first, bb.second, false); break;
   }
   default: // Nothing to do
      break;
   }

   return res;
}

// Spin piece
void GamePiece::spin()
{
   switch(this->form)
   {
   case Tetrimino::I :
   {
      if(this->ori == Orientation::UP || this->ori == Orientation::DOWN)
         this->addto_position( 1, -1);
      else
         this->addto_position(-1,  1);
      break;
   }

   case Tetrimino::O :
   {
      // Nothing to do
      break;
   }

   case Tetrimino::T :
   {
      switch(this->ori)
      {
      case Orientation::UP:    { this->addto_position( 0,  1); break;}
      case Orientation::LEFT:  { this->addto_position( 1, -1); break;}
      case Orientation::DOWN:  { this->addto_position(-1,  0); break;}
      case Orientation::RIGHT: { this->addto_position( 0,  0); break;}
      }
      break;
   }

   default :
   {
      if(this->ori == Orientation::UP || this->ori == Orientation::DOWN)
         this->addto_position(-1,  1);
      else
         this->addto_position( 1, -1);
      break;
   }
   }

   // next orientation for any piece
   switch(this->ori)
   {
   case Orientation::UP:    { this->ori = Orientation::LEFT;  break;}
   case Orientation::LEFT:  { this->ori = Orientation::DOWN;  break;}
   case Orientation::DOWN:  { this->ori = Orientation::RIGHT; break;}
   case Orientation::RIGHT: { this->ori = Orientation::UP;    break;}
   }
}

} /* namespace TwitterTetris */
