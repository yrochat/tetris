/**
 \file main.cpp
 \author steiner 
 \date Apr 13, 2016
**/

#include <cstdlib>
#include "GameState.h"
#include "GameAction.h"
#include "LaunchOptions.h"

using namespace std;
using namespace TwT;

#define DEBUG 0

int main(int argc, char** argv)
{
   // Read command line to find appropriates actions
   LaunchOptions options(argc, argv);

   if(!options.directExit())
   {
      try
      {
         // Read or define game properties
#if DEBUG
         std::cout << "Properties ..." << std::endl;
#endif
         GameProperties properties(options.properties());

         // Read last state from disk or generate new game
#if DEBUG
         std::cout << "Read state ..." << std::endl;
#endif
         GameState  current_state( options.last_state() , properties);

         // Generate a new state by applying the action to current state
#if DEBUG
         std::cout << "Make an action ..." << std::endl;
#endif
         GameState  next_state(current_state, GameAction( options.action() ));

         // Export the new state computed from last action
#if DEBUG
         std::cout << "Save state ..." << std::endl;
#endif
         next_state.export_xml(options.last_state());

         // Generate image with new state
#if DEBUG
         std::cout << "Export png ..." << std::endl;
#endif
         next_state.export_png(options.img_file() + ".png"); // Gen a new image in current_game repository

         // Convert image to gif
         string cmd = "convert " + options.img_file() + "*.png  " + options.img_file() + ".gif ; rm " + options.img_file() + "*.png";
         system(cmd.c_str());

         // Export state to ascii
         if(options.ascii_export())
         {
#if DEBUG
            std::cout << "Export ascii ..." << std::endl;
#endif
            next_state.export_ascii(options.ascii_file());
         }
      }
      catch(const std::bad_file_access& ex)
      {
         cout << "ERROR : " << ex.what() << endl;
      }
   }

   return 0;
}


