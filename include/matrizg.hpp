// *********************************************************************
// **
// ** Gestión de matrices cuadradas usando tuplas
// ** (declaraciones e implementaciones de plantillas de clases)
// **
// ** Copyright (C) 2014 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// 


#ifndef MATRIZG_HPP
#define MATRIZG_HPP

#include <cmath>
#include <cassert>
#include <iostream>

#include "tuplasg.hpp"

// *********************************************************************
//
// plantilla de clase: MatrizCG
// clase para matrices cuadradas genéricas
//
// *********************************************************************

template< class T, unsigned n > 
class MatrizCG
{
   public:
   TuplaG< TuplaG<T,n>, n > mat ;
   
   MatrizCG<T,n> ()  {}  // constructor por defecto inline (no inicializa)
   
   // conversion a un puntero de lectura/escritura de tipo: T* 
   // ( T* p = matriz )
   inline operator  T * ()  ;
   
   // conversion a un puntero de solo lectura de tipo: const T* 
   // ( const T* p = matriz )
   inline operator  const T * ()  const ;
   
   // acceso de lectura a una fila
   // (devuelve puntero al primer elemento de una columna)
   //inline const TuplaG<T,n> & operator() ( const unsigned i ) const ;
   
   // acceso de lectura/escritura a una fila: 
   //inline TuplaG<T,n> & operator[] ( int i )  ;
   
   // componer esta matriz con otra por la derecha
   inline MatrizCG<T,n> operator * ( const MatrizCG<T,n> & der ) const ;
   
   // acceso de solo lectura usando fila,columna: T x = m(fil,col)
   inline const T & operator()( const unsigned fil, const unsigned col ) const ;
   
   // acceso de lectura/escritura usando fila,columna: T x = m(fil,col)
   inline T & operator()( const unsigned fil, const unsigned col )  ;
   
   // multiplicar esta matriz por una tupla por la derecha
   inline TuplaG<T,n> operator * ( const TuplaG<T,n>  & t ) const ;
   
   // multiplicar esta matriz por una tupla por la derecha (con una dimesnión menos)
   // (se añade un 1, se multiplica, y luego se le quita la ultima componente)
   inline TuplaG<T,n-1> operator * ( const TuplaG<T,n-1>  & t ) const ;
} ;

// escritura de una matriz en un ostream
template< class T, unsigned n > inline 
std::ostream & operator <<  ( std::ostream & os, const MatrizCG<T,n> & m ) ; 


// *********************************************************************
// implementaciones in-line

#include "matrizg_impl.hpp"

// *********************************************************************


typedef MatrizCG<float,4> Matriz4f ;
typedef MatrizCG<double,4> Matriz4d ;

//inline operator * ( const MatrizCG




// *********************************************************************
#endif
