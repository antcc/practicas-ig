// *********************************************************************
// **
// ** Gestión de 'shaders' (implementación)
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

#include "aux.hpp"
#include "shaders.hpp"

//**********************************************************************
// gestion de 'shaders'
//
//**********************************************************************

// lee un archivo completo como texto  y devuelve una cadena terminada
// en cero con el contenido
// (el resultado está en el heap y debería borrarse con 'delete [] p')

char * LeerArchivo( const char * nombreArchivo )
{
	using namespace std ;

	ifstream file( nombreArchivo, ios::in|ios::binary|ios::ate );

	if ( ! file.is_open())
	{  cout << "imposible abrir archivo para lectura (" << nombreArchivo << ")" << endl << flush ;
		exit(1);
	}

	size_t	numBytes	= file.tellg();
	char *	bytes		= new char [numBytes+2];

	file.seekg (0, ios::beg);
	file.read (bytes, numBytes);
	file.close();

	bytes[numBytes]=0 ;
	bytes[numBytes+1]=0 ;

	return bytes ;
}


//----------------------------------------------------------------------

void VerErroresEnlazar( GLuint idProg )
{
   using namespace std ;
   const GLsizei  maxt = 1024L*10L ;
   GLsizei        tam ;
   GLchar         buffer[maxt] ;
   GLint          ok ;

	glGetProgramiv( idProg, GL_LINK_STATUS, &ok );
	if ( ok == GL_TRUE )
      return ;

   glGetProgramInfoLog( idProg, maxt, &tam, buffer);
   cout << "error al enlazar:" << endl
        << buffer << flush
        << "programa abortado" << endl << std::flush ;
   exit(1);
}

//----------------------------------------------------------------------

void VerErroresCompilar( GLuint idShader )
{

   using namespace std ;
   const GLsizei maxt = 1024L*10L ;
   GLsizei       tam ;
   GLchar        buffer[maxt] ;
   GLint         ok ;

	glGetShaderiv( idShader, GL_COMPILE_STATUS, &ok );
	if ( ok == GL_TRUE )
      return ;

   glGetShaderInfoLog( idShader, maxt, &tam, buffer);
   cout << "error al compilar:" << endl
        << buffer << flush
        << "programa abortado" << endl << std::flush ;
   exit(1);
}

//----------------------------------------------------------------------
// crea, carga y compila un shader nuevo
// si hay algún error, aborta
// si todo va bien, devuelve el código de opengl del shader

GLuint CompilarShader( const char * nombreArchivo, GLenum tipoShader )
{
   using namespace std ;
	GLuint idShader ; // resultado: identificador de shader

   CError() ;

   // crear shader nuevo, obtener identificador (tipo GLuint)
	idShader = glCreateShader( tipoShader );

   // leer archivo fuente de shader en memoria, asociar fuente al shader
   //cout << "compilando shader '" << nombreArchivo << "' ..... " << flush ;
	const GLchar * fuente = LeerArchivo( nombreArchivo );
   glShaderSource( idShader, 1, & fuente, NULL );
   delete [] fuente ; fuente = NULL ;

   // compilar y comprobar errores
   glCompileShader( idShader );
   VerErroresCompilar( idShader );

	// hecho
   CError() ;
	//cout << "ok" << endl << flush ;
	return idShader ;
}

//----------------------------------------------------------------------

GLuint CrearPrograma( const char * nomArchFragmentShader, const char * nomArchVertexShader )
{
   using namespace std ;
   CError() ;

   // crear y compilar shaders, crear el programa
	GLuint
      idFragShader = CompilarShader( nomArchFragmentShader, GL_FRAGMENT_SHADER ),
      idVertShader = CompilarShader( nomArchVertexShader,   GL_VERTEX_SHADER   ),
      idProg       = glCreateProgram();

	// asociar shaders al programa
	glAttachShader( idProg, idFragShader );
	glAttachShader( idProg, idVertShader );

   //cout << "enlazando programa ..... " << flush ;

#ifdef USAR_GVA
cout << endl << "HACIENDO BINDS--GVAs" << endl << flush ;
   CError() ;
   glBindAttribLocation( idProg, 0, "av_posicion");
   glBindAttribLocation( idProg, 1, "av_normal");
   glBindAttribLocation( idProg, 2, "av_coordText");
   CError() ;
#endif

	// enlazar programa
	glLinkProgram( idProg );
	VerErroresEnlazar( idProg );

#ifdef USAR_GVA
   // test
   const GLint
      locp = glGetAttribLocation( idProg, "av_posicion" ),
      locn = glGetAttribLocation( idProg, "av_normal" );
   cout << "### locp == " << locp << ", locn == " << locn << endl << flush ;
   assert( locp == 0 );
   assert( locn == 1 );
#endif

   CError() ;
   // ya está:
   //cout << "ok." << endl << flush ;
   return idProg ;
}
//----------------------------------------------------------------------

int LeerLocation( GLuint idProg, const char * nombre )
{
   CError() ;
   const int loc = glGetUniformLocation( idProg, nombre ); CError() ;
   if ( loc == -1 )
      std::cout << "advertencia: uniform '" << nombre << "' no está declarada en el programa o no se usa en la salida" << std::endl << std::flush ;
   return loc ;
}
//----------------------------------------------------------------------

void asignarUniform( GLuint idProg, const char * nombre, const int valor )
{
   using namespace std ;
   const int loc = LeerLocation(idProg,nombre) ;
   if ( loc != -1 )
   {  glUniform1i( loc, valor );
      //cout << "uniform (int) asignado, nombre==" << nombre << endl << flush ;
      CError() ;
      //cout << "despues de error" << endl << flush ;
   }
}

//----------------------------------------------------------------------

void asignarUniform( GLuint idProg, const char * nombre, const unsigned valor )
{
   const int loc = LeerLocation(idProg,nombre) ;
   if ( loc != -1 )
   {  glUniform1i( loc, (int)valor );
      CError() ;
   }
}

//----------------------------------------------------------------------

void asignarUniform( GLuint idProg, const char * nombre, const float valor )
{
   const int loc = LeerLocation(idProg,nombre) ;
   if ( loc != -1 )
   {  glUniform1f( loc, valor );
      CError() ;
   }
}
//----------------------------------------------------------------------

void asignarUniform( GLuint idProg, const char * nombre, const Matriz4f & mat )
{
   const int loc = LeerLocation(idProg,nombre) ;
   if ( loc != -1 )
   {  glUniformMatrix4fv( loc, 1, GL_FALSE, mat );
      CError() ;
   }
}
// ---------------------------------------------------------------------

void asignarUniform( GLuint idProg, const char * nombre, const Tupla3f & tupla )
{
   const int loc = LeerLocation(idProg,nombre) ;
   if ( loc != -1 )
   {  glUniform3fv( loc, 1, tupla );
      CError() ;
   }
}
// ---------------------------------------------------------------------

void asignarUniform( GLuint idProg, const char * nombre, int n, const Tupla3f tuplas[] )
{
   using namespace std ;
   const int loc = LeerLocation(idProg,nombre) ;
   if ( loc != -1 )
   {  glUniform3fv( loc, n, (tuplas[0]) );
      //cout << "uniform (Tupla3f[]) asignado, nombre==" << nombre << endl << flush ;
      CError() ;
      //cout << "despues de error" << endl << flush ;
   }
}
// ---------------------------------------------------------------------

void asignarUniform( GLuint idProg, const char * nombre, const Tupla4f & tupla )
{
   const int loc = LeerLocation(idProg,nombre) ;
   if ( loc != -1 )
   {  glUniform4fv( loc, 1, tupla );
      CError() ;
   }
}
// ---------------------------------------------------------------------
