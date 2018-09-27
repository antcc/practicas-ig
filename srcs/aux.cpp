// *********************************************************************
// **
// ** Rutinas auxiliares:
// ** - Gestión de errores en general
// ** - Comprobación de errores de OpenGL (implementacion)
// ** - Inicialización de glew,
// ** - Comprobacion de la versión de OpenGL
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
// *********************************************************************



#include "aux.hpp"


// *********************************************************************
// gestion de errores

void GestionarError( const char * msg, const char * nomArchivo, int linea )
{
   std::cout
         << std::endl
         << "-----------------------------------------------------------" << std::endl
         << "Error detectado:" << std::endl
         << "   descripción : " << msg  << "." << std::endl
         << "   archivo     : " << nomArchivo << std::endl
         << "   línea       : " << linea << std::endl
         << "-----------------------------------------------------------" << std::endl
         << "programa abortado." << std:: endl
         << std::flush ;

    exit(1) ;
}

// -----------------------------------------------------------------------------
// códigos y descripciones de errores detectado por 'glGetError'

const unsigned numErrors = 5 ;


const GLenum errCodes[numErrors] =
{
   GL_NO_ERROR ,
   GL_INVALID_ENUM ,
   GL_INVALID_VALUE ,
   GL_INVALID_OPERATION ,
   //GL_INVALID_FRAMEBUFFER_OPERATION ,  // REVISAR (pq no está definido con glfw?)
   GL_OUT_OF_MEMORY
} ;

const char * errDescr[numErrors] =
{
   "Error when trying to report an error: no error has been recorded",
   "An unacceptable value is specified for an enumerated argument",
   "A numeric argument is out of range",
   "The specified operation is not allowed in the current state",
   //"The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete (i.e. the return value from 'glCheckFramebufferStatus' is not GL_FRAMEBUFFER_COMPLETE)",
   "There is not enough memory left to execute the command"
} ;


const char * errCodeString[numErrors] =
{
   "GL_NO_ERROR",
   "GL_INVALID_ENUM",
   "GL_INVALID_VALUE",
   "GL_INVALID_OPERATION",
   //"GL_INVALID_FRAMEBUFFER_OPERATION",
   "GL_OUT_OF_MEMORY"
} ;

// ---------------------------------------------------------------------
// devuelve descripción de error dado el código de error opengl

std::string ErrorDescr( GLenum errorCode )
{
   int iErr = -1 ;
   for ( unsigned i = 0 ; i < numErrors ; i++ )
   {  if ( errCodes[i] == errorCode)
      {  iErr = i ;
         break ;
      }
   }
   if ( iErr == -1 )
      return std::string("Error when trying to report an error: error code is not a valid error code for 'glGetError'") ;
   return std::string( errDescr[iErr] ) ;
}

std::string ErrorCodeString( GLenum errorCode )
{
   int iErr = -1 ;
   for ( unsigned i = 0 ; i < numErrors ; i++ )
   {  if ( errCodes[i] == errorCode)
      {  iErr = i ;
         break ;
      }
   }
   if ( iErr == -1 )
      return std::string("** invalid error code **") ;
   return std::string( errCodeString[iErr] ) ;
}

//----------------------------------------------------------------------

void CompruebaErrorOpenGL( const char * nomArchivo, int linea )
{
   const GLint codigoError = glGetError() ;

   if ( codigoError != GL_NO_ERROR )
   {  std::cout
         << std::endl
         << "Detectado error de OpenGL. Programa abortado." << std::endl
         << "   archivo (linea) : " << QuitarPath(nomArchivo) << " (" << linea << ")" << std::endl
         << "   código error    : " << ErrorCodeString( codigoError )  << std::endl
         << "   descripción     : " << ErrorDescr( codigoError )  << "." << std::endl
         << std::endl << std::flush ;
      exit(1);
   }
}

//----------------------------------------------------------------------

void CalculaVersion(  const char *str, int & major, int & minor )
{
   // podría hacerse así, pero las primeras versiones (1.5-) al parecer no tienen GL_MAJOR_VERSION.... :-(
      // glGetIntegerv( GL_MAJOR_VERSION, &major ) ;
      // glGetIntegerv( GL_MINOR_VERSION, &minor ) ;

   using namespace std ;
   const
      std::string ver(str) ;
   size_t //auto
      blanco = ver.find_first_of(' ') ;
   if ( blanco == string::npos )
      blanco = ver.length() ;

   const size_t //auto
      punto = ver.find_first_of('.') ;
   assert( punto != string::npos ); // salta si no encuentra el punto
   assert( punto+1 < blanco );

   const string
      smajor = ver.substr( 0, punto ),
      sminor = ver.substr( punto+1, blanco-punto-1 );

   major = atoi(smajor.c_str());
   minor = atoi(sminor.c_str());
}

// ---------------------------------------------------------------------
// quita el path de un nombre de archivo con path

std::string QuitarPath( const std::string & path_arch )
{
   //const size_t

   const size_t // auto
   	barra = path_arch.find_last_of('/') ;
   if ( barra == std::string::npos ) // no tiene '/' (devolver tal cual)
      return path_arch ;
   else
      return path_arch.substr( barra+1 );

}

// ---------------------------------------------------------------------
// devuelve true si se soporta al menos una versión de OpenGL, se
// proporciona el número de version dividido en "major" y "minor"

bool SoportaGL( const int min_major, const int min_minor )
{
   static GLint major,minor ;
   static bool primera = true ;

   if ( primera )
   {
      // parser de la cadena (GL_VERSION)
      using namespace std ;
      CalculaVersion( (const char *) glGetString(GL_VERSION), major, minor ) ;
      cout << "calculada version de OpenGL: " << major << "." << minor << " (" << glGetString(GL_VERSION) << ")" << endl << flush ;
      primera = false ;
   }

   if ( min_major < major )
      return true;
   if ( min_major == major )
      if ( min_minor <= minor )
         return true;
   return false ;
}


// ---------------------------------------------------------------------

bool SoportaGLSL( const int min_major, const int min_minor )
{
   static GLint major,minor ;
   static bool primera = true ;

   if ( primera )
   {
      // parser de la cadena (GL_VERSION)
      using namespace std ;
      CalculaVersion( (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION), major, minor ) ;
      cout << "calculada version de GLSL: " << major << "." << minor << " (" << glGetString(GL_SHADING_LANGUAGE_VERSION) << ")" << endl << flush ;
      primera = false ;
   }

   if ( min_major < major )
      return true;
   if ( min_major == major )
      if ( min_minor <= minor )
         return true;
   return false ;
}


//----------------------------------------------------------------------

void Inicializa_GLEW(  ) // necesario para Linux y resto de SS.OO. 
{
#ifndef OSX
   static bool primera = true ;

   if ( ! primera )
      return ;
   primera = false ;

   using namespace std ;
#ifndef GLEW_OK
   Error("no se han incluido los headers de GLEW correctamente, usa '#include <aux.hpp>' para incluir símbolos de OpenGL/GLFW/GLEW") ;
#else
   GLenum codigoError = glewInit();
   if ( codigoError != GLEW_OK ) // comprobar posibles errores
   {
      const std::string errmsg =
         std::string( "Imposible inicializar ’GLEW’, mensaje recibido: ") +
         std::string( (const char *)glewGetErrorString(codigoError) ) ;
      Error(errmsg.c_str()) ;
   }
   else
      cout << "librería GLEW inicializada correctamente." << endl << flush ;
#endif
#else
   // en macOS (símbolo OSX definido) no es necesario usar 'glew'
#endif
}

// ---------------------------------------------------------------------

void InformeOpenGL(  )
{
   GLint acc_r, acc_g, acc_b ;
   glGetIntegerv( GL_ACCUM_RED_BITS,   &acc_r );
   glGetIntegerv( GL_ACCUM_GREEN_BITS, &acc_g );
   glGetIntegerv( GL_ACCUM_BLUE_BITS,  &acc_b );

   using namespace std ;
   cout  << "datos de versión e implementación de OpenGL" << endl
         << "    implementación de   == " << glGetString(GL_VENDOR)  << endl
         << "    hardware            == " << glGetString(GL_RENDERER) << endl
         << "    version de OpenGL   == " << glGetString(GL_VERSION) << endl
         << "    version de GLSL     == " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl
         << "    bits buff.accum     == " << "(" << acc_r << "," << acc_g << "," << acc_b << ")" << endl
         << flush ;
}

// *****************************************************************************
// funciones para dibujar los ejes, con conos, cilindros y esferas:

// ----------------------------------------------------------------------------
// declaración e inicialización del objeto tipo cuadrica ('qu_ptr')

static GLUquadric * qu_ptr = nullptr ; // objeto quadrica (para glusphere y glucone)

void CrearObjetoCuadrica()
{
   // generar el objeto 'cuadrica' (una sola vez)
   if ( qu_ptr == nullptr )
      qu_ptr = gluNewQuadric() ;

   assert( qu_ptr != nullptr );
}
// ----------------------------------------------------------------------------
// dibuja un cono cuyo eje es el eje Z y el disco de la base tiene centro en el origen
// 'base' es el radio de la base, y 'height' es la altura (en el eje Z)

void DibujarCono( GLdouble base, GLdouble height, GLdouble slices, GLdouble stacks )
{
   CrearObjetoCuadrica();
   gluCylinder( qu_ptr, base, 0.0, height, slices, stacks );
}
// ----------------------------------------------------------------------------
// dibuja una esfera, se proporciona el radio, el número de meridianos (slices)
// y el de paralelos (stacks)

void DibujarEsfera( GLdouble radius, GLdouble slices, GLdouble stacks )
{
   CrearObjetoCuadrica();
   gluSphere( qu_ptr, radius, slices, stacks );

}

// ----------------------------------------------------------------------------
// dibuja un cilindro, en el eje Z, entre Z=0 y Z=1, 'radius' es el radius

void DibujarCilindro( GLdouble radius, GLdouble height, int slices, int stacks )
{
   CrearObjetoCuadrica();
   gluCylinder( qu_ptr, radius, radius, height, slices, stacks );
}
// ----------------------------------------------------------------------------
// dibuja el eje Z

void DibujarEjeZ()
{
   const GLfloat
   	zmed     = 0.93,  // z fin del cilindro
   	rad_fle  = 0.023 ;  // radio del cono

   DibujarCilindro( rad_fle*0.3, zmed, 16,8 );

   glPushMatrix() ;
   	glTranslatef( 0.0, 0.0, zmed );
   	DibujarCono( rad_fle, 1.0-zmed, 16,16 );
   glPopMatrix();

   glBegin(GL_LINES);
   	glVertex3f( 0.0, 0.0, -50.0 );
   	glVertex3f( 0.0, 0.0, +50.0 );
   glEnd();
}


//----------------------------------------------------------------------

void DibujarEjesSolido()
{
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glMatrixMode( GL_MODELVIEW );
   glDisable( GL_LIGHTING );
   glDisable( GL_TEXTURE_2D );
   glShadeModel( GL_FLAT );

   // eje X, color rojo
   glColor3f( 1.0, 0.0, 0.0 );
   glPushMatrix() ;
      glRotatef( 90.0, 0.0,1.0,0.0 );
      DibujarEjeZ() ;
   glPopMatrix();

   // eje Y, color verde
   glColor3f( 0.0, 0.5, 0.0 );
   glPushMatrix() ;
      glRotatef( -90.0, 1.0, 0.0, 0.0 );
      DibujarEjeZ() ;
   glPopMatrix();

   // eje Z, color azul
   glColor3f( 0.0, 0.0, 1.0 );
   DibujarEjeZ() ;

   // bola en el origen, negra
   glColor3f(0.0,0.0,0.0);
   DibujarEsfera( 0.04, 16, 8 );
}

// ---------------------------------------------------------------------
// dibuja los ejes utilizando la primitiva grafica de lineas

void DibujarEjesLineas()
{
   const float long_ejes = 300.0 ;

   // establecer modo de dibujo a lineas (podría estar en puntos)
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

   // dibujar tres segmentos
   glBegin(GL_LINES);
      // eje X, color rojo
      glColor3f( 1.0, 0.0, 0.0 );
      glVertex3f( -long_ejes, 0.0, 0.0 );
      glVertex3f( +long_ejes, 0.0, 0.0 );
      // eje Y, color verde
      glColor3f( 0.0, 1.0, 0.0 );
      glVertex3f( 0.0, -long_ejes, 0.0 );
      glVertex3f( 0.0, +long_ejes, 0.0 );
      // eje Z, color azul
      glColor3f( 0.0, 0.0, 1.0 );
      glVertex3f( 0.0, 0.0, -long_ejes );
      glVertex3f( 0.0, 0.0, +long_ejes );
   glEnd();

   // bola en el origen, negra
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glColor3f(0.0,0.0,0.0);
   DibujarEsfera( 0.01, 8, 8 );

}
