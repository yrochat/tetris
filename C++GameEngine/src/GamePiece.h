/**
 * \brief
 *
 **/
/**
 \file GamePiece.h
 \author steiner 
 \date Apr 13, 2016
 **/

#include "core.h" 
#include "matrix.h"

#ifndef GAMEPIECE_H_
#define GAMEPIECE_H_

namespace TwitterTetris
{
class GamePiece
{
private:
   Tetrimino    form { Tetrimino::I };
   Orientation  ori  { Orientation::UP };

   // The position is the first lower left pixel of the bounding box of the piece
   int          LLRpos { 0 }; // Which row
   int          LLCpos { 0 }; // Which col

public:
   GamePiece(Tetrimino _f = Tetrimino::I, Orientation _o = Orientation::UP, int posR = 0, int posC = 0 ) : form {_f}, ori {_o}, LLRpos {posR}, LLCpos {posC} {}

   // Accessors
   Tetrimino      tetrimino() const { return form; }
   Orientation    orientation() const { return ori; }
   std::pair<int,int> pos() const { return std::pair<int,int>(LLRpos, LLCpos); }

   // Generate a new piece with a random form
   static GamePiece random_piece()
   {
      std::random_int_generator rand(0, AVAILABLE_TETRIMINI.size()-1);
      return GamePiece(AVAILABLE_TETRIMINI[rand()]);
   }

   // Compute bounding box of a piece according to form and orientation
   std::pair<int,int> boundingbox() const;

   // Change position of the piece
   void set_position(int x, int y)   { this->LLRpos = x; this->LLCpos = y; }
   void addto_position(int x, int y) { this->LLRpos += x; this->LLCpos += y; }

   // Return the matrix of the occupied space of the piece in the bounding box
   std::matrix<bool> occupied_pixels() const;

   void spin();     // Spin piece clockwise
   void antispin(); // Spin the piece anticlockwise

private:
   // Serialization with cereal
   friend class cereal::access;

   template <class Archive>
   void serialize( Archive & ar )
   {
      ar( cereal::make_nvp("Tetrimino", form),
          cereal::make_nvp("Orientation", ori),
          CEREAL_NVP(LLRpos), CEREAL_NVP(LLCpos) );
   }
};

} /* namespace TwitterTetris */

#endif /* GAMEPIECE_H_ */
