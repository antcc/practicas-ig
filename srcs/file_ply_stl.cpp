// *********************************************************************
// **
// ** Función ply::read (declaraciones)
// **
// ** Carlos Ureña - 2012- 2013
// **
// ** Más información y versión actualizada en:
// ** http://lsi.ugr.es/curena/varios/plys
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
// *********************************************************************


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <assert.h>

#include "file_ply_stl.hpp"

// modelos ply en:
   // http://graphics.im.ntu.edu.tw/~robin/courses/cg03/model/
   // http://people.sc.fsu.edu/~jburkardt/data/ply/ply.html
   // univ stanford ?


namespace ply
{

using namespace std ;

//**********************************************************************
// constantes y funciones auxiliares:

const streamsize tam_buffer = streamsize(10L)*streamsize(1024L) ;

void read_nvc
(
   const unsigned       nvc,                // entrada: número de vértices por cara
   const char *         nombre_archivo_pse, // entrada: nombre de archivo
   std::vector<float> & vertices,           // salida:  vector de coords. de vert.
   std::vector<int> &   caras               // salida:  vector de triángulos (índices)
);

void abrir_archivo( string & nombre_archivo, ifstream & src ) ;
void leer_cabecera( const unsigned nvc, ifstream & src,
                    unsigned & num_vertices, unsigned & num_caras,
                    const bool lee_num_caras ) ;
void error( const char *msg_error ) ;
void leer_vertices(  unsigned num_vertices, vector<float> & vertices,
                     ifstream & src  ) ;
void leer_caras( const unsigned nvc, unsigned num_vertices,
                 unsigned num_caras, vector<int> & caras,
                 ifstream & src  ) ;

//**********************************************************************
// funcion principal de lectura

void read
(
   const char *    nombre_archivo_pse,
   vector<float> & vertices,
   vector<int> &   caras
)
{  read_nvc( 3, nombre_archivo_pse, vertices, caras ) ; // por defecto se leen tres vértices por cara
}

//**********************************************************************

void read_4vc
(
   const char *    nombre_archivo_pse,
   vector<float> & vertices,
   vector<int> &   caras
)
{  read_nvc( 4, nombre_archivo_pse, vertices, caras ) ; //
}

//**********************************************************************

void read_nvc
(
   const unsigned  nvc,
   const char *    nombre_archivo_pse,
   vector<float> & vertices,
   vector<int> &   caras
)
{
   unsigned
      num_vertices = 0,
      num_caras   = 0 ;
   ifstream
      src ;
   string
      na = nombre_archivo_pse ;


   if ( na.substr( na.find_last_of(".")+1 ) != "ply" )
      na += ".ply" ;

   abrir_archivo( na, src ) ;
   leer_cabecera( nvc, src, num_vertices, num_caras, true ) ;
   leer_vertices( num_vertices, vertices, src ) ;
   leer_caras( nvc, num_vertices, num_caras, caras, src ) ;

   //cout << "archivo ply '" << na << "' leido: núm. vértices == " << num_vertices << ", núm caras == " << num_caras << endl << flush ;
}

//**********************************************************************

void read_vertices
(  const char *    nombre_archivo_pse,
   vector<float> & vertices
)
{

   unsigned
      num_vertices = 0,
      num_caras   = 0 ;
   ifstream
      src ;
   string
      na = nombre_archivo_pse ;

   if ( na.substr( na.find_last_of(".")+1 ) != "ply" )
      na += ".ply" ;

   abrir_archivo( na, src ) ;
   leer_cabecera( 3, src, num_vertices, num_caras, false ) ;
   leer_vertices( num_vertices, vertices, src ) ;

   //cout << "archivo ply leido (únicamente vértices)" << endl << flush ;
}


//**********************************************************************

void leer_vertices( unsigned num_vertices, vector<float> & vertices, ifstream & src )
{
   char
      buffer[unsigned(tam_buffer)];
   string
      token ;

   // leer vértices:

   vertices.resize( num_vertices*3 );

   //cout << "  leyendo " << num_vertices << " vértices ...." << endl << flush ;

   for( long long iv = 0 ; iv < num_vertices ; iv++ )
   {
      if ( src.eof() )
         error("fin de archivo prematuro en la lista de vértices");

      long double x,y,z ;

      src >> x >> y >> z ;
      //cout << "vertex #" << iv << " readed: (" << x << "," << y << "," << z << ")" << endl ;

      src.getline(buffer,tam_buffer); // ignore more properties, so far ...

      // add new vertex
      long long base = iv*3 ;
      vertices[base+0] = x ;
      vertices[base+1] = y ;
      vertices[base+2] = z ;
   }
   //cout << "  fin de la lista de vértices" << endl << flush ;
}

//**********************************************************************

void leer_caras
(
   const unsigned nvc,
   unsigned num_vertices,
   unsigned num_caras,
   vector<int> & caras,
   ifstream & src
)
{
   char
      buffer[unsigned(tam_buffer)];
   string
      token ;

   assert( nvc > 2 ) ; // tipicamente, 3 o 4.

   //cout << "  leyendo " << num_caras << " caras ...." << endl << flush ;

   caras.resize( num_caras*nvc );

   for( long long ifa = 0 ; ifa < num_caras ; ifa++ )
   {
      if ( src.eof() )
         error("fin de archivo prematuro en la lista de caras");

      unsigned nv ;
      src >> nv ;
      //cout << "reading face #" << ifa << " with " << nv << " vertexes: " ;

      if ( nv != nvc )
         error("encontrada una cara con un número de vértices distinto del prefijado.");

      long long
         iv[nvc] ;
      const long long
         base = ifa*nvc ;

      // bucle que lee los índices de vértices en una línea
      for ( unsigned ivc = 0 ; ivc < nvc ; ivc++ )
      {  src >> iv[ivc] ;
         if ( num_vertices <= iv[ivc] )
            error("encontrado algún índice de vértice igual o superior al número de vértices");
         caras[ base+ivc ] = iv[ivc] ;
      }

      src.getline(buffer,tam_buffer); // ignore more properties, so far ...
   }
   //cout << "  fin de la lista de caras." << endl ;
}

//**********************************************************************

void leer_cabecera
(
   const unsigned nvc,
   ifstream &src,
   unsigned & num_vertices,
   unsigned & num_caras,
   const bool lee_num_caras
)
{
   char
      buffer[unsigned(tam_buffer)];
   string
      token ;
   unsigned
      state = 0; // 0 antes de leer 'element vertex' (o 'element face'), 1 antes de leer 'element face', 2 después
   bool
      en_cabecera = true ;
   long long int
      nv = 0,
      nc = 0 ;

   // leer cabecera:

   while( en_cabecera )
   {
      if ( src.eof() )
         error("fin de archivo prematuro antes de end_header");

     src >> token ;

     if ( token == "end_header" )
     {  if ( state != 2 )
           error("no encuentro 'element vertex' o 'element face' en la cabecera");
        src.getline(buffer,tam_buffer);
        en_cabecera = false ;
     }
     else if ( token == "comment" )
     {  src.getline(buffer,tam_buffer);
        //cout << "  comment: " << buffer << endl ;
     }
     else if ( token == "format" )
     {  src >> token ;
        if ( token != "ascii" )
        {  string msg = string("el formato del ply no es 'ascii' es '")+token+"', no lo puedo leer" ;
           error(msg.c_str());
        }
        src.getline(buffer,tam_buffer);
     }
     else if ( token == "element" )
     {  src >> token ;
        if ( token == "vertex" )
        {  if ( state != 0 )
              error("la línea 'element vertex' va después de 'element face'");
           src >> nv ;
           //cout << "  numero de vértices == " << nv << endl ;
           state = lee_num_caras ? 1 : 2 ;
        }
        else if ( lee_num_caras && token == "face" )
        {  if ( state != 1 )
              error("'element vertex' va después de 'element face'");
           src >> nc ;
           //cout << "  número de caras == " << nc << endl ;
           state = 2 ;
        }
        else
        {  //cout << "  elemento '" + token + "' ignorado." << endl ;
        }
        src.getline(buffer,tam_buffer);
     }
     else if ( token == "property" )
     {  src.getline(buffer,tam_buffer); // ignore properties, so far ...
     }
   } // end of while( en_cabecera )

   if ( nv <= 0 )
      error("no se ha encontrado el número de vértices, o bien es 0 o negativo");


   if ( lee_num_caras ) if ( nc <= 0 )
      error("no se ha encontrado el número de caras, o bien es 0 o negativo");

   if ( nv > numeric_limits<int>::max() )
      error("el número de vértices es superior al valor 'int' más grande posible.");

   if ( lee_num_caras )
   if ( nc > numeric_limits<int>::max() )
      error("el número de caras es superior al valor 'int' más grande posible.");

   num_vertices = unsigned(nv) ;
   num_caras    = unsigned(nc) ;
}

//**********************************************************************


void abrir_archivo( string & nombre_archivo, ifstream & src )
{
   using namespace std ;
   char buffer[unsigned(tam_buffer)];
   string token ;

   src.open( nombre_archivo.c_str() ) ; // abrir (¿en modo lectura?)

   if ( ! src.is_open() )
   {
      string msg = string("no puedo abrir el archivo '") + nombre_archivo + "' para lectura." ;
      error(msg.c_str());
   }

   src >> token ;

   if ( token != "ply" )
      error("el archivo de entrada no comienza con 'ply'");

   src.getline(buffer,tam_buffer);

   //cout << "leyendo archivo ply '" + nombre_archivo + "'" << endl ;
}


//**********************************************************************

void error( const char *msg_error )
{
   using namespace std ;
   cout << "error leyendo archivo ply: " << msg_error << endl
        << "programa terminado." << endl
        << flush ;

   exit(1);
}




} // fin namespace _file_ply
