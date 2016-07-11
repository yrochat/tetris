/**
 * \brief A class to handle rgba colors, dealing with double, integer or hexadecimal color codes and values.
 *
 **/
/**
 \file color.h
 \author steiner 
 \date Apr 13, 2016
 **/

#ifndef COLOR_H_
#define COLOR_H_

#include "core.h"

namespace std
{
class color
{
private:
   double red   = 1.0;
   double green = 1.0;
   double blue  = 1.0;
   double alpha = 1.0;
public:
   color() {}

   color(int r, int g, int b)
   : red { static_cast<double>(r % 256) / 255.0 },green { static_cast<double>(g % 256) / 255.0 }, blue { static_cast<double>(b % 256) / 255.0 }
   {}

   color(double r, double g, double b)
   : red { min(1.0, max(0.0, r)) }, green { min(1.0, max(0.0, g)) }, blue { min(1.0, max(0.0, b)) }
   {}
   color(double r, double g, double b, double a)
   : red { min(1.0, max(0.0, r)) }, green { min(1.0, max(0.0, g)) }, blue { min(1.0, max(0.0, b)) }, alpha {min(1.0, max(0.0, a))}
   {}

   color(string hexacode)
   {
      if(hexacode.size() != 6)
      {
         throw invalid_argument("color(string hexacode) : hexacode.size should be 6 characters. hexacode = " + hexacode);
      }

      int r0 = char_to_int(hexacode[0]); int r1 = char_to_int(hexacode[1]);
      int g0 = char_to_int(hexacode[2]); int g1 = char_to_int(hexacode[3]);
      int b0 = char_to_int(hexacode[4]); int b1 = char_to_int(hexacode[5]);

      if(r0 < 0 || r1 < 0 || g0 < 0 || g1 < 0 || b0 < 0 || b1 < 0)
      {
         throw invalid_argument("color(string hexacode) : Invalid character detected in hexacode (allowed are 0-9, A-F and a-f). hexacode = " + hexacode);
      }

      // Call int constructor with interpreted hexadecimal code
      color newcolor(r0*16 + r1, g0*16+g1, b0*16+b1); *this = newcolor;
   }

   double r() const {return red;}
   double g() const {return green;}
   double b() const {return blue;}
   double a() const {return alpha;}
private:
   int char_to_int(char c)
   {
      int res = -1;
      switch(c) {
      case '0': { res = 0; break; }
      case '1': { res = 1; break; }
      case '2': { res = 2; break; }
      case '3': { res = 3; break; }
      case '4': { res = 4; break; }
      case '5': { res = 5; break; }
      case '6': { res = 6; break; }
      case '7': { res = 7; break; }
      case '8': { res = 8; break; }
      case '9': { res = 9; break; }
      case 'A': { res = 10; break; }
      case 'B': { res = 11; break; }
      case 'C': { res = 12; break; }
      case 'D': { res = 13; break; }
      case 'E': { res = 14; break; }
      case 'F': { res = 15; break; }
      case 'a': { res = 10; break; }
      case 'b': { res = 11; break; }
      case 'c': { res = 12; break; }
      case 'd': { res = 13; break; }
      case 'e': { res = 14; break; }
      case 'f': { res = 15; break; }
      default : { res = -1; }
      }
      return res;
   }

private:
   // Serialization with cereal
   friend class cereal::access;

   template <class Archive>
   void serialize( Archive & ar )
   {
      ar( CEREAL_NVP(red), CEREAL_NVP(green), CEREAL_NVP(blue) );
   }

};

const color white(1.0,1.0,1.0);
const color black(0.0,0.0,0.0);

const std::color grey3(0.3, 0.3, 0.3);
const std::color grey5(0.5, 0.5, 0.5);
const std::color grey7(0.7, 0.7, 0.7);

} /* namespace std */

#endif /* COLOR_H_ */
