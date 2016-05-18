/**
 * \brief A class to handle current state of the game.
 *
 **/
/**
 \file GameState.h
 \author steiner 
 \date Apr 13, 2016
 **/

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "GameArea.h"
#include "GameAction.h"
#include "GamePiece.h"
#include "GameProperties.h"

namespace TwitterTetris
{
class GameState
{
private:
   GameProperties properties;

   GameArea     draw_area;
   GamePiece    current_piece;
   GamePiece    next_piece;

   bool         isGameOver = false;

   int          level = 1;
   int          score = 0;
   int          lines = 0;

public:
   GameState() {}
   GameState(const std::string filename, const GameProperties& _properties);
   GameState(const GameState& from_state, const GameAction& action);

   // xml serialization of state
   void export_xml(const std::string filename) const;

   // Export the state in the console or in a file
   void export_ascii(const std::string filename) const
   {
      if(filename.size() > 0) {
         std::ofstream file(filename.c_str());
         this->export_ascii_stream(file);
      }
      else
         this->export_ascii_stream();
   }

   // Export the state to an image showing the situation
   void export_png(const std::string filename) const;

private:
   // Serialization with cereal
   friend class cereal::access;

   template <class Archive>
   void serialize( Archive & ar )
   {
      ar( cereal::make_nvp( "Drawing_Area", draw_area),
          cereal::make_nvp( "Current_Piece", current_piece),
          cereal::make_nvp( "Next_Piece", next_piece),
          cereal::make_nvp( "Game_Over", isGameOver),
          cereal::make_nvp( "Level", level),
          cereal::make_nvp( "Score", score),
          cereal::make_nvp( "Lines", lines) );
   }

   // Place a piece in an available initial position or declare game_over
   void enter_piece(GamePiece& _piece);

   // Generate a new piece
   void new_piece() {
      this->current_piece = this->next_piece;
      this->next_piece = GamePiece::random_piece();

      // Place current_piece in initial position
      this->enter_piece(this->current_piece);
   }

   // Generate a state for a new game
   void NewGame();

   // Export ascii state to a given stream
   void export_ascii_stream(std::ostream & sout = std::cout) const;
};

} /* namespace TwitterTetris */

#endif /* GAMESTATE_H_ */
