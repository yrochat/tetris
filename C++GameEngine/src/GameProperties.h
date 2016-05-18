/**
 * \brief
 *
 **/
/**
 \file GameProperties.h
 \author steiner 
 \date Apr 13, 2016
 **/

#ifndef GAMEPROPERTIES_H_
#define GAMEPROPERTIES_H_

#include "color.h"

namespace TwitterTetris
{
class GameProperties
{
private:
   // Base dimensions of the area where the game is played
   // Most tetris implementations use a 20x10 matrix
   int mGameAreaNbRows { 20 };
   int mGameAreaNbCols { 10 };

   // The colors of those pieces
   std::vector<std::color> PieceColors { std::color("00FFFF"), std::color("0000FF"), std::color("FF9933"),
                                         std::color("FFFF00"), std::color("00FF00"), std::color("990099"),
                                         std::color("FF0000") };

   std::color mBackGroundColor { std::color("AAAAAA") };

   // This is used for ASCII export
   std::vector<char> PieceChars { 'o', 'x', 'z', '%', '+', 's', '#' };

   // This is used for score computation
   std::vector<int> PointPerNbLines { 0, 100, 200, 400, 1200 };

public:
   // Default constructor
   GameProperties() {}
   GameProperties(std::string filename)
   {
      std::ifstream file(filename.c_str());
      if(file.good()) {
         this->load_from_xml(filename);
      }
      else {
         this->save_to_xml(filename);
      }

      if(mGameAreaNbRows < 10) mGameAreaNbRows = 10;
      if(mGameAreaNbCols < 6)  mGameAreaNbCols = 6;
   }

   // Accessors
   int GameAreaNbRows() const { return mGameAreaNbRows; };
   int GameAreaNbCols() const { return mGameAreaNbCols; };

   std::color BackGroundColor() const { return mBackGroundColor; }

   std::color PieceColor(const Tetrimino piece) const {
      std::color res = PieceColors[0];
      switch(piece) {
      case Tetrimino::I: {res = PieceColors[0]; break;}
      case Tetrimino::J: {res = PieceColors[1]; break;}
      case Tetrimino::L: {res = PieceColors[2]; break;}
      case Tetrimino::O: {res = PieceColors[3]; break;}
      case Tetrimino::S: {res = PieceColors[4]; break;}
      case Tetrimino::T: {res = PieceColors[5]; break;}
      case Tetrimino::Z: {res = PieceColors[6]; break;}
      default: {res = this->BackGroundColor(); }
      }
      return res;
   }

   char PieceChar(const Tetrimino piece) const {
      char res = PieceChars[0];
      switch(piece) {
      case Tetrimino::I: {res = PieceChars[0]; break;}
      case Tetrimino::J: {res = PieceChars[1]; break;}
      case Tetrimino::L: {res = PieceChars[2]; break;}
      case Tetrimino::O: {res = PieceChars[3]; break;}
      case Tetrimino::S: {res = PieceChars[4]; break;}
      case Tetrimino::T: {res = PieceChars[5]; break;}
      case Tetrimino::Z: {res = PieceChars[6]; break;}
      default: {res = ' '; }
      }
      return res;
   }

   int Points(int nblines) const { return PointPerNbLines[nblines % PointPerNbLines.size()]; }

   // Mutators
   void ChangePieceColors(std::vector<std::color> new_colors)
   {
      // Loop on 7 first colors in input and change GameColors accordingly
      for(size_t k=0; k < std::min(new_colors.size(), PieceColors.size()); ++k)
         this->PieceColors[k] = new_colors[k];
   }

   // Save and load from XML
   void save_to_xml(std::string filename)
   {
      std::ofstream file(filename.c_str());

      if(file.good())
      {
         cereal::XMLOutputArchive archive(file);
         GameProperties TMP = *this; archive(TMP);
      }
      else
      {
         throw std::bad_file_access(filename);
      }
   }

   void load_from_xml(std::string filename)
   {
      std::ifstream file(filename.c_str());
      if(file.good())
      {
         cereal::XMLInputArchive archive(file);
         GameProperties TMP;  archive(TMP); *this = TMP;
      }
   }

private:
   // Serialization with cereal
     friend class cereal::access;

     template <class Archive>
     void serialize( Archive & ar )
     {
        ar( cereal::make_nvp( "AreanbRows", mGameAreaNbRows),
            cereal::make_nvp( "AreanbCols", mGameAreaNbCols),
            CEREAL_NVP(PieceColors),
            cereal::make_nvp( "Background_color", mBackGroundColor),
            cereal::make_nvp( "ASCII_Characters", PieceChars),
            cereal::make_nvp( "Scoring_system", PointPerNbLines) );
     }
};

} /* namespace TwitterTetris */

#endif /* GAMEPROPERTIES_H_ */
