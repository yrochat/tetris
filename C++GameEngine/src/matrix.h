/**
 * \brief A template class for MxN matrices
 *
 **/
/**
 \file matrix.h
 \author steiner 
 \date Apr 13, 2016
 **/

#ifndef MATRIX_H_
#define MATRIX_H_

#include "core.h"

namespace std
{
template <typename Object>
class matrix
{
public:
   matrix() {}
   matrix( int rows, int cols ) : array( rows )
   {
      for( auto & thisRow : array )
         thisRow.resize( cols );
   }

   matrix( int rows, int cols, const Object val ) : array( rows )
   {
      for( auto & thisRow : array )
         thisRow.resize( cols, val );
   }

   matrix( vector<vector<Object>> & v ) : array{ v }
   { }
   matrix( vector<vector<Object>> && v ) : array{ std::move( v ) }
   { }

   const vector<Object> & operator[]( int row ) const
   { return array[ row ]; }
   vector<Object> & operator[]( int row )
   { return array[ row ]; }

   int nbrows( ) const
   { return array.size( ); }
   int nbcols( ) const
   { return nbrows( ) ? array[ 0 ].size( ) : 0; }
private:
   vector<vector<Object>> array;

   // Serialization with cereal
   friend class cereal::access;

   template <class Archive>
   void serialize( Archive & ar )
   {
      ar( CEREAL_NVP(array) );
   }

};
} /* namespace std */

#endif /* MATRIX_H_ */
