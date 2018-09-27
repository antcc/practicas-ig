// *********************************************************************
// **
// ** Gestion de las matrices de vista y proyección, y el viewport
// ** (clase "Camara").
// ** Declaraciones.
// **
// ** Copyright (C) 2017 Carlos Ureña - obtenido de las transparencias del tema 3
// **

#ifndef CAMARA_HPP
#define CAMARA_HPP

#include "tuplasg.hpp"
#include "matrices-tr.hpp"


// *********************************************************************
// clase: MarcoCoorVista

class MarcoCoorVista  // marco de coordenadas ({\bf cartesiano}) de la vista
{

   public:
   Tupla3f  org,      // origen del marco de coordenadas
            eje[3]  ; // ejes (0=X, 1=Y, 2=Z) del marco de cc. (ortonormales)
            // APUNTES: deberían llamarse: matriz_MaL (mundo a locales) (matriz de Vista)
                                       //  matriz_LaM  (locales-->mundo)
   Matriz4f matrizVistaInv, // matriz marco de vista --> mundo
            matrizVista; // matriz de vista: coords. mundo --> coords. marco de vista

   // foco en origen, mirando hacia Z-, vup = Y+
   MarcoCoorVista() ;
   // constructor: mismos parámetros que {\ttbf gluLookAt} )
   MarcoCoorVista( const Tupla3f & pfoco, const Tupla3f & paten,
                   const Tupla3f & pvup );
} ;

// *********************************************************************
// clase: ViewFrustum

class ViewFrustum
{
   public:
   bool      persp ;      // {\ttbf true} para perspectiva, {\ttbf false} para ortográfica
   float     left, right, // extension en X ({\ttbf left} < {\ttbf right})
             bottom, top, // extension en Y ({\ttbf bottom} < {\ttbf top})
             near, far ;  // extension en Z (rama negativa) ($0$ < {\ttbf near} < {\ttbf far})
   Matriz4f  matrizProy ; // matriz de proyección $P$: cc. cámara ==> cc. recortado

   ViewFrustum() ; // view-frustum ortográfico, de lado 2, centro en (0,0,0)

   // crea view-frustum perspectiva, mismos parámetros que {\ttbf gluPerspective}
   ViewFrustum( float hfovy_grad, float aspect, float zNear, float zFar );
} ;

// *********************************************************************
// clase: Camara

class Camara
{
   public:
   MarcoCoorVista  mcv ; // marco de coordenadas de la vista
   ViewFrustum     vf  ; // parámetros de la proyección

   Camara() ;            // usa constructores por defecto para {\ttbf mc} y {\ttbf vf}

   void activar() ;  // fijar matrices {\ttbf MODELVIEW} y {\ttbf PROJECTION} de OpenGL
} ;

// *********************************************************************
// clase: Viewport

class Viewport
{
  public:
  int       org_x, org_y, // origen en pixels (esquina inferior izquierda)
            ancho, alto ; // dimensiones en pixels (núm. columnas, núm. filas)
  float     ratio_yx ;    // == alto/ancho (relación de aspecto)
  Matriz4f  matrizVp ,    // matriz de viewport ( pasa: NDC ==> DC )
            matrizVpInv ; // matriz inversa ( pasa: DC ==> NDC )
  // constructor
  Viewport() ; // crea viewport de 512 x 512 con origen en (0,0)
  Viewport( int p_org_x, int p_org_y, int p_ancho, int p_alto );
} ;

// *********************************************************************
// clase ParTransf

class ParTransf
{
  public:
  Camara    cam ; // Cámara activa actualmente para la visualización
  Viewport  vp  ; // {\it viewport} actualmente en uso
  ParTransf() {} ;  // usa constructores por defecto (no hace nada)
} ;

#endif
