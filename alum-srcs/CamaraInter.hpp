// *********************************************************************
// **
// ** Gestion de cámaras interactivas (clase CamaraInteractiva)
// ** Declaraciones.
// **
// ** Copyright (C) 2017 Carlos Ureña
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


#ifndef CAMARA_INTER_HPP
#define CAMARA_INTER_HPP

#include "tuplasg.hpp"
#include "matrices-tr.hpp"
#include "Camara.hpp"

// *********************************************************************
//
// Clase CamaraInteractiva
//
// *********************************************************************

class CamaraInteractiva : public Camara
{
 public:
 bool    examinar; // modo: {\bf true} --> examinar, {\bf false} --> primera persona (inicialmente {\bf false})
 bool    perspectiva ; // true --> camara perspectiva, false --> cámara ortográfica
 float   ratio_yx_vp;  // aspect ratio del viewport (alto/ancho)
 float   longi,     // en modo examinar: ángulo (en radianes) en torno al eje Y (longitud) (inic. 0)
         lati ;     // en modo examinar: ángulo (en radianes) en torno al eje X (latitud) (inic. 0))
 Tupla3f aten ;     // en modo examinar: punto de atención (inicialmente el origen+dist)
 float   dist ,     // en modo examinar: distancia entre el punto de atención y el observador
         hfov_grad, // para cámaras perspectiva: angulo de apertura horizontal
         dx,dy ;    // desplazamientos 'subpixel' para antialiasing (0 por inicialmente)


 // constructor de cámaras interactivas, los parámetros son:
 //
 //    * examinar_ini: fija modo examinar (true) o modo primera persona (false)
 //    * ratio_yx_vp_ini: ratio del viewport (alto/ancho)
 //    * longi_ini_grad, lati_ini_grad: longitud y latitud iniciales (en grados)
 //    * aten_ini: punto de atención inicial
 //    * pers_ini: fija proy. perspectiva (true) u ortográfica (false)
 //
 CamaraInteractiva( bool examinar_ini, float ratio_yx_vp_ini,
                    float longi_ini_grad, float lati_ini_grad,
                    const Tupla3f & aten_ini, bool pers_ini,
                    float hfov_grad_ini = 80.0,
                    float dist_ini      = 1.8 );

 //  métodos para manipular (desplazar o rotar) la cámara
 void moverHV( float nh, float nv ); // desplazar o rotar la cámara en horizontal/vertical
 void desplaZ( float nz );         // desplazar en el eje Z de la cámara (hacia adelante o hacia detrás)

 // calcula view-frustum (vf) (la matriz de proyección) usando:
 //    perspectiva, dist, ratio_yx_vp
 void calcularViewfrustum(  );

 // calcula el marco de referencia de la camara y la matriz de vista (mcv), usando:
 //     longi, lati, dist, aten
 void calcularMarcoCamara() ;

 // métodos para cambiar de modo
 void modoExaminar( const Tupla3f & pAten ); // fija punt. aten. y activa modo examinar
 void modoExaminar();                        // pasa a modo examinar (mantiene p.aten.)
 void modoPrimeraPersona() ;                 // pasa al modo primera persona


 // recalcular las matrices del marco de coorda de camara, a partir de:
 // mcv.org, mcv.eje[X/Y/Z]
 void recalcularMatrMCV();



} ;

#endif
