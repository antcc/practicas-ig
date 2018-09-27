// *********************************************************************
// **
// ** Gestión de tuplas genéricas de coordenadas o colores 
// ** (declaraciones de plantillas de clases)
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


#ifndef TUPLASG_HPP
#define TUPLASG_HPP

#include <cmath>
#include <cassert>
#include <iostream>

// constantes para acceder a las componentes de las tuplas
const unsigned 
   X = 0, 
   Y = 1, 
   Z = 2,
   
   R = 0,
   G = 1,
   B = 2 ;
   
// definir alias de 'unsigned int' cuyo descriptor tiene un solo token
typedef unsigned int uint ; 

   
// *********************************************************************
//
// plantilla de clase: TuplaG
// clase para tuplas genéricas
//
// *********************************************************************

template< class T, unsigned n > 
class TuplaG
{
   private:
   T coo[n] ;  // vector de valores escalares
   
   public:
   // constructor por defecto: no hace nada
   inline TuplaG();
   
   // constructor usando un array C++
   inline TuplaG( const T * org ) ;
   
   // acceso de lectura/escritura a un elemento (v[i]=x, x=v[i]) 
   //T & operator [] (const unsigned i) ; 
   
   // acceso de solo lectura a un elemento ( x=v(i) )
   const T & operator () (const unsigned i) const ; 
   
   // acceso de lectura-escritura a un elemento ( v(i)=x )
   T & operator () (const unsigned i) ;
   
   // conversion a un puntero de lectura/escritura de tipo T* ( T* p = tupla )
   operator  T * ()  ;
   
   // conversion a un puntero de solo lectura de tipo: const  T* ( const T* p = tupla )
   operator  const T * ()  const ;
   
   // suma componente a componente ( v1=v2+v3 )
   TuplaG<T,n> operator + ( const TuplaG & der ) const ; 
   
   // resta componente a componente ( v1=v2-v3 )
   TuplaG<T,n> operator - ( const TuplaG & der ) const ; 
   
   // devuelve tupla negada ( v1 = -v2 )
   TuplaG<T,n> operator - (  ) const ; 
   
   // mult. por escalar por la derecha ( v1=v2*a )
   TuplaG<T,n> operator * ( const T & a ) const ; 
   
   // division por escalar ( v1=v2/a )
   TuplaG<T,n> operator / ( const T & a ) const ; 
   
   // producto escalar (dot)  a = v1.dot(v2)
   T dot( const TuplaG<T,n> & v2 ) const ;
   
   // operador binario para producto escalar a = v1|v2 ;
   T operator | ( const TuplaG & der ) const ;
   
   // obtener longitud al cuadrado
   T lengthSq( ) const ;
   
   // obtener una copia normalizada
   TuplaG<T,n> normalized() const ;
} ;

//----------------------------------------------------------------------
// operadores y funciones no miembros

// mult. por escalar por la izquierda ( v1=a*v2 )
template< class T, unsigned n >
inline TuplaG<T,n> operator *  ( const T & a, const  TuplaG<T,n> & der ) ; 

// escritura de un vector en un ostream
template< class T, unsigned n >
inline std::ostream & operator <<  ( std::ostream & os, const TuplaG<T,n> & der ) ; 

// *********************************************************************
//
// Plantilla de clase: TuplaG2
// especialización parcial para tuplas de 2 elementos 
// (define constructores específicos)
//
// *********************************************************************

template< class T >
class TuplaG2 : public TuplaG<T,2>
{
   public:
   
   // constructores: por defecto
   TuplaG2() ;
   TuplaG2( const T & c0, const T & c1 ) ;
   TuplaG2( const TuplaG<T,2> & ini );
   void operator = ( const TuplaG<T,2> & der ) ;
} ;



// *********************************************************************
//
// Plantilla de clase: TuplaG3
// especialización parcial para tuplas de 3 elementos 
// (define constructores específicos)
//
// *********************************************************************

template< class T >
class TuplaG3 : public TuplaG<T,3>
{
   public:
   
   // constructores: por defecto
   TuplaG3() ;
   TuplaG3( const T & c0, const T & c1, const T & c2 ) ;
   TuplaG3( const TuplaG<T,3> & ini );
   void operator = ( const TuplaG<T,3> & der ) ;
   void operator = ( const TuplaG<T,4> & der ) ; // asignar ignorando ultimo
   
   // producto vectorial (cross)  a = v1.cross(v2)
   TuplaG3<T> cross( const TuplaG3<T> & v2 ) const ;
} ;


// *********************************************************************
//
// Plantilla de clase: TuplaG4
// especialización parcial para tuplas de 4 elementos 
// (define constructores específicos)
//
// *********************************************************************

template< class T >
class TuplaG4 : public TuplaG<T,4>
{
   public:
   
   // constructores: por defecto
   TuplaG4() ;
   TuplaG4( const T & c0, const T & c1, const T & c2, const T & c3 ) ;
   TuplaG4( const TuplaG<T,4> & ini );
   void operator = ( const TuplaG<T,4> & der ) ;
} ;



// *********************************************************************
// tipos concretos:

typedef TuplaG2<float>  Tupla2f ;
typedef TuplaG2<double> Tupla2d ;
typedef TuplaG2<uint>   Tupla2u ;
typedef TuplaG2<int>    Tupla2i ;

typedef TuplaG3<float>  Tupla3f ;
typedef TuplaG3<double> Tupla3d ;
typedef TuplaG3<uint>   Tupla3u ;
typedef TuplaG3<int>    Tupla3i ;

typedef TuplaG4<float>  Tupla4f ;
typedef TuplaG4<double> Tupla4d ;
typedef TuplaG4<int>    Tupla4i ;
typedef TuplaG4<uint>   Tupla4u ;



// *********************************************************************
// incluir implementaciones 'inline'

#include "tuplasg_impl.hpp"


// *********************************************************************
#endif
