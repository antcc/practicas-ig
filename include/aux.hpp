// *********************************************************************
// **
// ** Conjunto de funciones auxiliares
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
// *********************************************************************

#ifndef AUX_HPP
#define AUX_HPP

// includes de la librería estandard y otras
#include <algorithm>
#include <cassert>  // assert
#include <cctype>   // toupper
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <map>      // std::map<K,V>
#include <string>   // std::string
#include <vector>   // std::vector

// includes de OpenGL/Glut/Glew/Glu (dependen del S.O.)
#ifdef LINUX
//#pragma message "Usando includes para LINUX"
#include <GL/glew.h>   // nota: define GLEW_OK
#include <GLFW/glfw3.h>  // glfw (incluye OpenGL?)
#include <GL/glu.h>
#else
#ifdef OSX
//#pragma message "Usando includes para OSX"
#include <GLFW/glfw3.h>  // glfw (incluye OpenGL?)
#include <OpenGL/glu.h>
#else
#pragma error "No está definido el símbolo 'OSX' ni el símbolo 'LINUX'"
#endif
#endif


// activa uso de "generic vertex attributes"
//#define USAR_GVA

inline std::string aux_archs_folder()
{
   return std::string("../alum-archs/") ;
}


// ignora advertencias por uso de funciones obsoletas (sirve en OSX)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// ---------------------------------------------------------------------
// traza la ejecución
#define traza() { std::cout << "archivo: " << __FILE__ << ", linea: " << __LINE__ << std::endl << std:: flush ;}

// ---------------------------------------------------------------------
// comprobación de errores de OpenGL
#define CError() CompruebaErrorOpenGL(__FILE__,__LINE__)
void CompruebaErrorOpenGL( const char * nomArchivo, int linea ) ;

// ---------------------------------------------------------------------
// gestion de un error (revisar y quitar version en minusculas)
#define Error(msg)   GestionarError(msg,__FILE__,__LINE__)
void GestionarError( const char * msg, const char * nomArchivo, int linea ) ;

// ---------------------------------------------------------------------
// true solo si el rendering context activo soporta al menos una versión de openGL
bool SoportaGL( const int min_major, const int min_minor ) ;

// ---------------------------------------------------------------------
// true solo si el rendering context activo soporta al menos una versión de GLSL
bool SoportaGLSL( const int min_major, const int min_minor ) ;

//----------------------------------------------------------------------
// aborta si no se soporta una versión de OpenGL
void ExigirVersionGL( const int min_major, const int min_minor, const char * msg ) ;

//----------------------------------------------------------------------
// comprobar si está GLEW, si está inicializarlo, en otro caso error y aborta
void Inicializa_GLEW() ;

// ---------------------------------------------------------------------
// escribe en stdout características de opengl
void InformeOpenGL() ;

// ---------------------------------------------------------------------
// crear una ventana glut centrada en el escritorio
void CrearVentanaGLUT( const char * titulo, const  float fr_alto ) ;

// ---------------------------------------------------------------------
// quita el path de un nombre de archivo con path
std::string QuitarPath( const std::string & path_arch ) ;

// ----------------------------------------------------------------------------
// funciones auxiliares para dibujar los ejes

void CrearObjetoCuadrica() ;
void DibujarCono( GLdouble base, GLdouble height, GLdouble slices, GLdouble stacks );
void DibujarEsfera( GLdouble radius, GLdouble slices, GLdouble stacks ) ;
void DibujarCilindro( GLdouble radius, GLdouble height, int slices, int stacks );
void DibujarEjeZ();
void DibujarEjesSolido();
void DibujarEjesLineas();

#endif // AUX_HPP
