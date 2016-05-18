/**
 * \brief
 *
 **/
/**
 \file GameArea.h
 \author steiner 
 \date Apr 13, 2016
 **/

#ifndef GAMEAREA_H_
#define GAMEAREA_H_

#include "matrix.h"
#include "color.h"
#include "GamePiece.h"

namespace TwitterTetris
{
class GameArea
{
private:
   int AREA_NBROWS {20}; // Nbrows
   int AREA_NBCOLS {10}; // Nbcols

   std::matrix<Tetrimino>   areaUnit;

public:
   // Default constructor for a GameArea
   // Generate an empty pixel matrix with correct sizes
   // Pixel numerotation is the following :
   //
   // pos[r][c] means rth row of the area, cth col of the area
   //
   // We start from the bottom-left and go to the top-right part of area.

   GameArea(int rsize = 20, int csize = 10)
   : AREA_NBROWS(rsize), AREA_NBCOLS(csize), areaUnit(rsize, csize, Tetrimino::NONE)
   { }

   // Access area dimensions
   int nbrows() const { return AREA_NBROWS; }
   int nbcols() const { return AREA_NBCOLS; }

   // Check if a piece intersects the area
   bool intersect(const GamePiece& _piece) const;

   // Check if a piece is out of the area
   bool isout(const GamePiece& _piece) const
   {
      std::pair<int, int> pos = _piece.pos();
      std::pair<int, int> bb  = _piece.boundingbox();
      return (pos.first < 0 || pos.second < 0 || (pos.first + bb.first - 1) >= this->AREA_NBROWS || (pos.second + bb.second - 1) >= this->AREA_NBCOLS);
   }

   // Check if a piece is in the area
   bool isin (const GamePiece& _piece) const { return !this->isout(_piece); }

   // access operator
   Tetrimino operator()(int i, int j) const { return areaUnit[i][j]; }

   // Freeze piece into area
   void freeze(const GamePiece& _piece);

   // Int area for full rows - return nbfull rows found and erase full rows
   int fullrows();

private:
   // Serialization with cereal
   friend class cereal::access;

   template <class Archive>
   void serialize( Archive & ar )
   {
      ar( cereal::make_nvp("nbRows", AREA_NBROWS),
          cereal::make_nvp("nbCols", AREA_NBCOLS),
          CEREAL_NVP(areaUnit) );
   }

   // Erase a row and move down upper rows
   void erase_row(int r);
};

} /* namespace TwitterTetris */

#endif /* GAMEAREA_H_ */
