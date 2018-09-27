// *********************************************************************
// **
// ** Gestión de matrices cuadradas usando tuplas
// ** (implementaciones inline de plantillas de clases)
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


#ifndef MATRIZG_IMPL_HPP
#define MATRIZG_IMPL_HPP



// *********************************************************************
// implementaciones inline

// ---------------------------------------------------------------------
// conversion a un puntero de lectura/escritura de tipo T* ( T* p = m )
// (devuelve puntero al primer elemento de la primera columna)
template< class T, unsigned n > inline 
MatrizCG<T,n>::operator  T * ()  
{
   return mat[0] ; 
}

// ---------------------------------------------------------------------
// conversion a un puntero de solo lectura de tipo: const T* ( T* p = m )
// (devuelve puntero al primer elemento de la primera columna)
template< class T, unsigned n > inline 
MatrizCG<T,n>::operator  const T * ()  const
{
   return & (mat(0)(0)) ; 
}

//// ---------------------------------------------------------------------
//// acceso de lectura a una fila: 
//template< class T, unsigned n > inline 
//const TuplaG<T,n> & MatrizCG<T,n>::operator() ( const unsigned i ) const 
//{
   //return mat(i) ;
//}

////----------------------------------------------------------------------

//template< class T, unsigned n > inline 
//TuplaG<T,n> & MatrizCG<T,n>::operator[] ( int i ) 
//{
   //return mat[i] ;
//}

//----------------------------------------------------------------------

template< class T, unsigned n > inline 
std::ostream & operator <<  ( std::ostream & os, const MatrizCG<T,n> & m ) 
{
   const char * izq = "/||\\", * der = "\\||/" ;
   using namespace std ;
   for( unsigned fil = 0 ; fil < n ; fil++ )
   {  os << izq[fil] << " " ;
      for( unsigned col = 0 ; col < n ; col++ )
      {   os << m(fil,col) ;
          if ( col < n-1 ) os << " , " ;
      }
      os << " " << der[fil] << endl ;
   }
   return os ;
}

//----------------------------------------------------------------------
// componer una matriz con otra por la derecha

template< class T, unsigned n > inline
MatrizCG<T,n> MatrizCG<T,n>::operator * ( const MatrizCG<T,n> & der ) const 
{
   MatrizCG<T,n> res ;
   
   for( unsigned fil = 0 ; fil < n ; fil++ )
   for( unsigned col = 0 ; col < n ; col++ )
   {  res(fil,col) = 0.0f ;
      for( unsigned k = 0 ; k < n ; k++ )
         res(fil,col) += (*this)(fil,k)*der(k,col) ;
   }
   return res ;
}

// ---------------------------------------------------------------------
// acceso de solo lectura usando fila,columna: T x = m(fil,col)

template< class T, unsigned n > inline
const T & MatrizCG<T,n>::operator()( const unsigned fil, const unsigned col ) const 
{
   assert( fil < n );
   assert( col < n ) ;
   return mat(col)(fil) ;
}


// ---------------------------------------------------------------------
// acceso de lectura/escritura usando fila,columna: m(fil,col) = v 

template< class T, unsigned n > inline
T & MatrizCG<T,n>::operator()( const unsigned fil, const unsigned col ) 
{
   assert( fil < n );
   assert( col < n ) ;
   return mat(col)(fil) ;
}

// ---------------------------------------------------------------------
// multiplicar esta matriz por una tupla por la derecha
template< class T, unsigned n > inline 
TuplaG<T,n> MatrizCG<T,n>::operator * ( const TuplaG<T,n>  & t ) const 
{
   TuplaG<T,n>  res ;
   for( unsigned fil = 0 ; fil < n ; fil++ )
   {  res[fil] = 0.0 ;
      for( unsigned col = 0 ; col < n ; col++ )
         res(fil) += (*this)(fil,col) * t(col) ;
   }
   return res ;
}

// ---------------------------------------------------------------------
// multiplicar esta matriz por una tupla por la derecha (con una dimesnión menos)
// (se añade un 1, se multiplica, y luego se le quita la ultima componente)

template< class T, unsigned n > inline 
TuplaG<T,n-1> MatrizCG<T,n>::operator * ( const TuplaG<T,n-1>  & t ) const 
{
   TuplaG<T,n> t1 ;
   
   // añadir un 1 a t, crear 't1'
   for( unsigned i = 0 ; i < n-1 ; i++ )
      t1(i) = t(i) ;
   t1(n-1) = T(1); // añadir un uno al final
   
   //// multiplicar por 't1', obtener 'resn'
   //for( unsigned fil = 0 ; fil < n ; fil++ )
   //{  resn(fil) = 0.0 ;
      //for( unsigned col = 0 ; col < n ; col++ )
         //resn(fil) += (*this)(fil,col) * t1(col) ;
   //}
   
   TuplaG<T,n> res1 = (*this)*t1 ;
   
   // copiar 'resn' en 'res'
   TuplaG<T,n-1>  res ;
   for( unsigned i = 0 ; i < n-1 ; i++ )
      res(i) = res1(i) ;
      
   return res ;
}



   


// *********************************************************************
#endif
