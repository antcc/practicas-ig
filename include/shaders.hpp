// *********************************************************************
// **
// ** Gestión de 'shaders' (declaraciones)
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

#ifndef SHADERS_HPP
#define SHADERS_HPP

#include "matrices-tr.hpp"

// inicialización:
void   InicializaGLEW() ;

// cargar, compilar y enlazar shaders:
char * LeerArchivo( const char * nombreArchivo ) ;
void   VerErroresEnlazar( GLuint idProg ) ;
void   VerErroresCompilar( GLuint idShader ) ;
GLuint CompilarShader( const char * nombreArchivo, GLenum tipoShader ) ;
GLuint CrearPrograma( const char * nomArchFragmentShader, const char * nomArchVertexShader ) ;

// asignar un valor a un parámetro 'uniform'
void asignarUniform( GLuint idProg, const char * nombre, const int valor ) ;
void asignarUniform( GLuint idProg, const char * nombre, const unsigned valor ) ;
void asignarUniform( GLuint idProg, const char * nombre, const float valor ) ;
void asignarUniform( GLuint idProg, const char * nombre, const Matriz4f & mat ) ;
void asignarUniform( GLuint idProg, const char * nombre, const Tupla3f  & tupla ) ;
void asignarUniform( GLuint idProg, const char * nombre, const Tupla4f  & tupla ) ;
void asignarUniform( GLuint idProg, const char * nombre, int n, const Tupla3f tuplas[] );


#endif
