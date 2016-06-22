/**
 * \brief
 *
 **/
/**
 \file GameAction.h
 \author steiner 
 \date Apr 13, 2016
 **/

#ifndef GAMEACTION_H_
#define GAMEACTION_H_

#include "core.h"

namespace TwitterTetris
{
class GameAction
{
private:
   Action thisAction = Action::NONE;

public:
   // Default constructor
   GameAction() {}

   // Set action from character
   GameAction(const char c)
   {
      switch(c) {
      // no action
      case '0': { thisAction = Action::NONE;  break; }
      case '-': { thisAction = Action::NONE;  break; }
      case 'n': { thisAction = Action::NONE;  break; }

      // left
      case 'l': { thisAction = Action::LEFT;  break; }
      case '<': { thisAction = Action::LEFT;  break; }

      // right
      case 'r': { thisAction = Action::RIGHT; break; }
      case '>': { thisAction = Action::RIGHT; break; }

      // spin/up
      case 's': { thisAction = Action::SPIN;  break; }

      // anti-spin
      case 'a': { thisAction = Action::ANTISPIN;  break; }

      // fall (1 unit down)
      case 'd': { thisAction = Action::DOWN;  break; }

      // all the way down
      case 'f': { thisAction = Action::FALL;  break; }
      case '|': { thisAction = Action::FALL;  break; }

      default : { thisAction = Action::NONE; }
      }
   }

   // Set action from action
   GameAction(const Action a): thisAction{a} {}

   // Access action
   Action operator()() const { return thisAction; }

};

} /* namespace TwitterTetris */

#endif /* GAMEACTION_H_ */
