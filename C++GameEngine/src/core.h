/**
 * \brief
 *
 **/
/**
 \file core.h
 \author steiner 
 \date Apr 13, 2016
**/

#ifndef CORE_H_
#define CORE_H_

// Some basic includes we would use in most files
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <random>
#include <utility>
#include <chrono>

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>
#include <cereal/archives/xml.hpp> // XML serialization

namespace TwitterTetris
{
  // The list of available pieces
  //
  // I : xxxx | J : xxx | L : xxx | O : xx | S :  xx | T : xxx | Z : xx
  //          |       x |     x   |     xx |     xx  |      x  |      xx
  enum class Tetrimino { I, J, L, O, S, T, Z, NONE };
  const std::vector<Tetrimino> AVAILABLE_TETRIMINI {Tetrimino::I, Tetrimino::J, Tetrimino::L, Tetrimino::O, Tetrimino::S, Tetrimino::T, Tetrimino::Z};

  // The available orientation for each piece
  // Four orientations are available. The default is UP.
  //
  enum class Orientation { UP, LEFT, DOWN, RIGHT };

  // The list of available actions for a piece
  //
  // NONE  : no action         SPIN : turn the tetrimino clockwise                 ANTISPIN : turn the tetrimino anticlockwise
  // LEFT  : try to go left    DOWN : 1 unit down (slow fall)
  // RIGHT : try to go right   FALL : hard fall of the tetrimino all the way down
  enum class Action { NONE, LEFT, RIGHT, SPIN, ANTISPIN, DOWN, FALL };
}

namespace std {

class bad_file_access : public std::exception {
private:
   string filename;

public:
   explicit bad_file_access(string _filename): filename {_filename } {}
   virtual const char* what() const  throw() {
      string res {"file_access_failed_exception"};
      if(filename.size() > 0)
         res += " with file : " + filename;

      return res.c_str();
   }
};

class random_int_generator {
private:
   std::default_random_engine generator;
   std::uniform_int_distribution<int> uniform_distr;

public:
   random_int_generator(int lo, int hi)
   {
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      generator = std::default_random_engine(seed);
      uniform_distr = std::uniform_int_distribution<int>(lo, hi);
   }

   int operator()() {return uniform_distr(generator);}
};

/// Convert from numbers to string
template <typename T>
string to_string(T num)
{
   ostringstream out;
   out << num;

   return out.str();
}

} /* namespace std */


#endif /* CORE_H_ */
