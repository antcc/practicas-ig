// *********************************************************************
// **
// ** Gestion de las matrices de vista y proyección, y el viewport
// ** (clase "Camara").
// ** Implementacion
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

#include <cmath>
#include "aux.hpp"
#include "Camara.hpp"

const float epsilon = 1e-6 ;

// ---------------------------------------------------------------------
// funciones auxiliares para generar matrices:

// *********************************************************************
// clase: MarcoCoorVista
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------

// foco en origen, mirando hacia Z-, vup = Y+
MarcoCoorVista::MarcoCoorVista()
{
   org    = Tupla3f( 0.0, 0.0, 0.0 ) ;
   eje[X] = Tupla3f( 1.0, 0.0, 0.0 ) ;
   eje[Y] = Tupla3f( 0.0, 1.0, 0.0 ) ;
   eje[Z] = Tupla3f( 0.0, 0.0, 1.0 ) ;

   matrizVista = MAT_Ident() ;
   matrizVistaInv = MAT_Ident() ;
}

// ---------------------------------------------------------------------

// constructor: mismos parámetros que {\ttbf gluLookAt} )
MarcoCoorVista::MarcoCoorVista( const Tupla3f & pfoco, const Tupla3f & paten,
                                const Tupla3f & pvup )
{
   const Tupla3f vpn      = pfoco-paten ;
   const float   vpn_lsq  = vpn.lengthSq(),
                 vup_lsq  = pvup.lengthSq();
   const Tupla3f ejex     = pvup.cross( vpn );
   const float   ejex_lsq = ejex.lengthSq();

   assert( epsilon < vpn_lsq );
   assert( epsilon < vup_lsq );
   assert( epsilon < ejex_lsq );

   eje[X] = ejex/sqrt( ejex_lsq );
   eje[Z] = vpn/sqrt( vpn_lsq );
   eje[Y] = eje[Z].cross( eje[X] );
   org    = pfoco ;

   matrizVista = MAT_Vista( eje, org ) ;
   matrizVistaInv = MAT_Vista_inv( eje, org );
}


// *********************************************************************
// clase: ViewFrustum

// ---------------------------------------------------------------------
// view-frustum ortográfico, de lado 2, centro en (0,0,0)
ViewFrustum::ViewFrustum()
{
   persp  = false ;
   left   = -1.0 ;
   right  = +1.0 ;
   bottom = -1.0 ;
   top    = +1.0 ;

   near   = -1.0 ;
   far    = +1.0 ;

   matrizProy = MAT_Ortografica( left, right, bottom, top, near, far );
}

// ---------------------------------------------------------------------
// crea view-frustum perspectiva, mismos parámetros que {\ttbf gluPerspective}

ViewFrustum::ViewFrustum( float hfovy_grad, float aspect, float zNear, float zFar )
{
   assert( epsilon < zNear );
   assert( zNear+epsilon < zFar );
   assert( epsilon < hfovy_grad );
   assert( epsilon < aspect );

   const float hfovy_rad = (hfovy_grad*M_PI)/180.0 ;

   persp  = true ;
   top    = zNear*tan( 0.5*hfovy_rad ) ;
   bottom = -top ;
   right  = top*aspect ;
   left   = -right ;
   near   = zNear ;
   far    = zFar ;

   matrizProy = MAT_Frustum( left, right, bottom, top, near, far );
}


// *********************************************************************
// clase: Camara
// ---------------------------------------------------------------------
// usa constructores por defecto para {\ttbf mc} y {\ttbf vf}

Camara::Camara()
{

}


// ---------------------------------------------------------------------
// fijar matrices {\ttbf MODELVIEW} y {\ttbf PROJECTION} de OpenGL

void Camara::activar()
{
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glMultMatrixf( vf.matrizProy ); // matriz de proyección

   // fijar matriz de vista en OpenGL
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glMultMatrixf( mcv.matrizVista ); // matriz de vista
}




// *********************************************************************
// clase: Viewport



// ---------------------------------------------------------------------
// crea viewport de 512 x 512 con origen en (0,0)

Viewport::Viewport()
{
   org_x    = 0 ;
   org_y    = 0 ;
   ancho    = 512 ;
   alto     = 512 ;
   ratio_yx = float(alto)/float(ancho) ;

   matrizVp    = MAT_Viewport( org_x, org_y, ancho, alto );
   matrizVpInv = MAT_Viewport_inv( org_x, org_y, ancho, alto );
}

// ---------------------------------------------------------------------

Viewport::Viewport( int p_org_x, int p_org_y, int p_ancho, int p_alto )
{
   org_x    = p_org_x ;
   org_y    = p_org_y ;
   ancho    = p_ancho ;
   alto     = p_alto ;
   ratio_yx = float(alto)/float(ancho) ;

   matrizVp    = MAT_Viewport( org_x, org_y, ancho, alto );
   matrizVpInv = MAT_Viewport_inv( org_x, org_y, ancho, alto );
}
// *********************************************************************
