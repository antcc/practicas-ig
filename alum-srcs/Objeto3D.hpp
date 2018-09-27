// *********************************************************************
// **
// ** Informática Gráfica
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_OBJETO3D_HPP
#define IG_OBJETO3D_HPP

#include <string>          // usar std::string
#include "practicas.hpp"   // declaración de 'ContextoVis'

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class Objeto3D
{
   protected:
      std::string  nombre_obj ;    // nombre asignado al objeto
      Tupla3f      centro_oc ;     // punto central o representativo del objeto, en coordenadas de objeto
      int          identificador ; // identificador de este objeto
                                   // 0: no tiene identificador, -1: identificador del padre,
                                   // >0: tiene este identificador

   public:
      // pone el identificador a '0', siempre
      Objeto3D() ;

      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) = 0 ;

      // devuelve el nombre del objeto
      std::string leerNombre() ;

      // poner nombre al objeto
      void ponerNombre( const std::string & nuevoNombre );

      // devuelve el identificador del objeto:
      int leerIdentificador() ;

      // ponerle un identificador al objeto
      void ponerIdentificador( int nuevoIdent );

      // poner el centro del objeto:
      void ponerCentroOC( const Tupla3f & nuevoCentro );

      // leer el centro del objeto
      Tupla3f leerCentroOC();

      // (re)calcular centro del objeto, si es necesario
      virtual void calcularCentroOC() ;

      // destructor
      virtual ~Objeto3D();

      // método para buscar un objeto con un identificador y devolver
      // un puntero al objeto y el punto central
      //
      // resultado:
      //    true si se ha encontrado, false si no.
      //
      // parámetros de entrada:
      //    ident_busc: identificador a buscar
      //    modelview:  matriz de modelado del padre del objeto (pasa coords.loc. a WC)
      //                (en el nodo raíz es la matriz identidad)
      //
      // parámetros de salida
      //    objeto: si encontrado y no es nullptr: apunta al puntero al objeto
      //    centro_wc: punto central del nodo en coords. de mundo (si encontrado)

      virtual bool buscarObjeto( const int ident_busc,
         const Matriz4f & mmodelado, Objeto3D ** objeto, Tupla3f & centro_wc )  ;

} ;



#endif
