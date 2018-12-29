// *********************************************************************
// **
// ** Gestión de materiales y texturas (declaraciones)
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


#ifndef MATERIALES_HPP
#define MATERIALES_HPP

#include <vector>
#include "aux.hpp"
#include "tuplasg.hpp"
#include "jpg_imagen.hpp"


// *********************************************************************
// algunes declaraciones auxiliares importantes

class Material ;
class Textura  ;
typedef Tupla4f VectorRGB ;

// *********************************************************************
// clase para una pila de materiales

class PilaMateriales
{
   private:

   Material *              actual ;
   std::vector<Material *> pila ;

   public:

   PilaMateriales( ) ;
   void activarMaterial( Material * material );
   void activarActual();

   void push();
   void pop();
} ;

//**********************************************************************
// posibles modos de generacion de coords. de textura

typedef enum
{  mgct_desactivada,
   mgct_coords_objeto,
   mgct_coords_ojo
}
   ModoGenCT ;



// *********************************************************************
// Estructura ColoresMat
// ---------------------
// estructura con las reflectividades o colores de un material (+exp.brillo)

struct ColoresMat
{
   VectorRGB
      emision,    // color de la emisividad del material
      ambiente,   // reflectividad para la componente ambiental (M_A)
      difusa,     // reflectividad para la componente difusa (M_D)
      especular ; // reflectividad para la componente especular (M_S)
   float
      exp_brillo ; // exponente de brillo especular
} ;

// *********************************************************************
// Clase Textura:
// ---------------
// clase que encapsula una imagen de textura de OpenGL, así como los
// parámetros relativos a como se aplica  a las primitivas que se dibujen
// mientras está activa

class Textura
{
   public:

   // carga una imagen de textura en la memoria de vídeo, e
   // inicializa los atributos de la textura a valores por defecto.
   Textura( const std::string & nombreArchivoJPG ) ;

   // libera la memoria dinámica usada por la textura, si hay alguna
   ~Textura() ;

   // activar una textura, por ahora en el cauce fijo
   void activar(  ) ;

   protected: //--------------------------------------------------------

   void enviar() ;    // envia la imagen a la GPU (gluBuild2DMipmaps)

   bool
      enviada ; // true si ha sido enviada, false en otro caso
   GLuint
      ident_textura ;// 'nombre' o identif. de textura para OpenGL
   jpg::Imagen *
      imagen ;       // objeto con los bytes de la imagen de textura
   ModoGenCT
      modo_gen_ct ;  // modo de generacion de coordenadas de textura
                     // (desactivadas si modo_gen_ct == mgct_desactivada)
   float
      coefs_s[4] ,   // si 'modo_gen_ct != desactivadas', coeficientes para la coord. S
      coefs_t[4] ;   // idem para coordenada T
} ;

// *********************************************************************
// clase: TexturaXY
// ---------------------------------------------------------------------
// textura con generación automática de coords de textura (s=x,t=y)

class TexturaXY : public Textura
{
   public: TexturaXY( const std::string & nom );
} ;

// *********************************************************************
// Clase Material
// ---------------------------------------------------------------------
// clase que encapsula los atributos de un material, incluyendo la
// textura del mismo.

class Material
{
   public:

   // crea un material con un color plano blanco sin textura
   // ni iluminación
   Material() ;

   // crea un material con un color plano sin textura ni iluminación
   // (el color se pasa como parámetro)
   Material( const float r, const float g, const float b );

   // crea un material con una textura asociada, y con la iluminación activada.
   // 'nombreArchivoJPG' nombre del archivo que contiene la imagen de text.
   Material( const std::string & nombreArchivoJPG ) ;

   // libera la memoria dinámica ocupada por el material
   ~Material() ;

   // crea un material usando textura y coeficientes: ka,kd,ks
   // (la textura puede ser NULL, la ilum. queda activada)
   Material( Textura * text, float ka, float kd, float ks, float exp ) ;

   // crea un material con un color único para las componentes ambiental y difusa
   // en el lugar de textura (textura == NULL)
   Material( const Tupla3f & colorAmbDif, float ka, float kd, float ks, float exp ) ;

   // activa un material (por ahora en el cauce fijo)
   void activar( ) ;

   // poner y leer el nombre del material
   void ponerNombre( const std::string & nuevo_nombre );
   std::string nombre() const ;

    //--------------------------------------------------------

   void coloresCero();// pone todos los colores y reflectividades a cero

   std::string nombre_mat ;  // nombre del material

   bool
      iluminacion ;  // true si el material requiere activar iluminación,
                     // false si requiere desactivarla
   Textura *
      tex ;          // si !=NULL, el material tiene esta textura
   VectorRGB
      color ;        // color del material cuando iluminacion=false
   ColoresMat
      del,           // reflectividades de caras delanteras, si iluminacion= true
      tra ;          // reflectividades de caras traseras, si iluminacion=true
} ;

//**********************************************************************
// Clase FuenteLuz
// ---------------
// clase que contiene los atributo de una fuente de luz OpenGL

class FuenteLuz
{
   public:

   // inicializa la fuente de luz
   //
   // p_longi_ini == valor inicial del ángulo horizontal en grados
   // p_lati_ini  == idem del ángulo vértical
   // p_color     == color de la fuente de luz (amb, dif y spec )
   FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const VectorRGB & p_color ) ;

   // cambia el estado de OpenGL de forma que a partir de la llamada
   // se usará esta fuente de luz en los calculos del MIL
   // (en cada momento puede haber varias fuentes activadas)
   void activar() ;

   // cambia los atributos de la instancia en respuesta a una pulsación
   // de una tecla 'especial' (según la terminología de 'glut')
   bool gestionarEventoTeclaEspecial( int key ) ;

   //-------------------------------------------------------------------
   // variables de instancia:

public:
    float
      longi,      // longitud actual de la fuente direccional (en grados, entre 0 y 360)
      lati ;      // latitud actual de la fuente direccional (en grados, entre -90 y 90)
protected:
   VectorRGB
      col_ambiente,  // color de la fuente para la componente ambiental
      col_difuso,    // color de la fuente para la componente difusa
      col_especular; // color de la fuente para la componente especular
   GLenum
      ind_fuente ;// indice de la fuente de luz en el vector, se asigna al insertarlo
   float
      longi_ini,  // valor inicial de 'longi'
      lati_ini ;  // valor inicial de 'lati'

   friend class ColFuentesLuz ;
} ;

//**********************************************************************
// Clase ConjuntoFuentes
// ---------------
// clase que contiene los atributos de una coleccion de fuentes de luz OpenGL

class ColFuentesLuz
{
   public:
   ColFuentesLuz() ; // crea la colección vacía
   ~ColFuentesLuz() ;
   void insertar( FuenteLuz * pf ) ; // inserta una nueva
   void activar( unsigned id_prog ); // activa las fuentes de luz
   FuenteLuz * ptrFuente( unsigned i ); // devuelve ptr a la fuente de luz numero i

   private:
   std::vector<FuenteLuz *> vpf ; // vector de punteros a fuentes
   GLint max_num_fuentes ;
} ;
#endif
