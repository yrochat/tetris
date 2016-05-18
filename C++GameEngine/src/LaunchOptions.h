/**
 * \brief
 *
 **/
/**
 \file LaunchOptions.h
 \author steiner 
 \date Apr 14, 2016
 **/

#include "core.h" 

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

namespace TwitterTetris
{
class LaunchOptions
{
private:
   char         maction = 'n'; // Available actions are 'n', 'l', 'r', 'u', 'd', 'f'
   std::string  mlast_state = ""; // File with last state saved
   std::string  mimg_file = "";
   bool         mdirectExit = false;
   bool         mascii_export = false; // Export in ascii format
   std::string  mascii_file = "";
   std::string  mproperties = "properties.xml"; // File for game properties

public:
   LaunchOptions(int argc, char* argv[]);

   char action() const {return maction;}
   std::string last_state() const { return mlast_state; }
   std::string img_file() const { return mimg_file; }
   bool   directExit() const { return mdirectExit; }
   bool   ascii_export() const { return mascii_export; }
   std::string ascii_file() const { return mascii_file; }
   std::string properties() const { return mproperties; }
};

} /* namespace TwitterTetris */

#endif /* COMMANDLINE_H_ */
