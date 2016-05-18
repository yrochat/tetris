/**
 * \brief
 *
 **/
/**
 \file LaunchOptions.cpp
 \author steiner 
 \date Apr 14, 2016
 **/

#include "LaunchOptions.h"

namespace TwitterTetris
{
LaunchOptions::LaunchOptions(int argc, char* argv[])
{
   // Copy command into strings for safer use
   std::vector<std::string> command_args;
   for(int i = 0; i < argc; ++i)
     {
        std::ostringstream text;
        text << argv[i];

        if(text.str().size() > 0)
           command_args.push_back(text.str());
     }

   // Loop on command line arguments
   for(uint i = 0; i < command_args.size(); ++i)
   {
      // Command options begins with -
      if(command_args[i] == std::string("--action") || command_args[i] == std::string("-a"))
      {
         // Next argument, if it exists is the action
         if(command_args.size() > i+1) this->maction = command_args[i+1][0];
      }

      if(command_args[i] == std::string("--last") || command_args[i] == std::string("-l"))
      {
         // Next argument, if it exists is the filename
         if(command_args.size() > i+1) this->mlast_state = command_args[i+1];
      }

      if(command_args[i] == std::string("--img"))
      {
         // Next argument, if it exists is the image filename
         if(command_args.size() > i+1) this->mimg_file = command_args[i+1];
      }

      if(command_args[i] == std::string("--ascii"))
      {
         this->mascii_export = true;
         // Next argument, if it exists is the ascii filename
         if(command_args.size() > i+1)
            if(command_args[i+1][0] != '-') this->mascii_file = command_args[i+1];
      }

      if(command_args[i] == std::string("--help"))
      {
         mdirectExit = true;
         std::cout << "Usage: TwitterTetris [options]" << std::endl
                   << "Options:" << std::endl << std::left

                   << std::setw(25) << " --help" << "Display this information and exit" << std::endl

                   << std::setw(25) << " --action, -a <char>" << "Define next action to be applied. Available actions are 'n' (no action)," << std::endl
                   << std::setw(25) << " " << "'l' (move left), 'r' (move right), 'u' (spin the piece), 'd' (fall all the way down), 'f' (fall 1 unit down)" << std::endl

                   << std::setw(25) << " --last, -l <file>" << "XML file with last available state" << std::endl

                   << std::setw(25) << " --img <file>" << "File name for the PNG image to generate" << std::endl

                   << std::setw(25) << " --ascii <optional file> " << "Display state with ascii characters in file or in console if no file supplied" << std::endl

                   << std::setw(25) << " --properties, -p <file> " << "If file exists, read properties. If not, write default properties to file." << std::endl;

      }
   }
}
} /* namespace TwitterTetris */
