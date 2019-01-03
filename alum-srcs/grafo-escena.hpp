// *********************************************************************
// **
// ** Gestión de una grafo de escena (declaraciones)
// ** Copyright (C) 2014-2017 Carlos Ureña
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


#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

#include "matrices-tr.hpp"
#include "materiales.hpp"
#include "Objeto3D.hpp"

// *********************************************************************
// declaración adelantada de estructura para un nodo del grafo de escena

class NodoGrafoEscena ;

// *********************************************************************
// tipo enumerado con los tipos de entradas del nodo del grafo de escena

enum class TipoEntNGE { objeto, transformacion, material, noInicializado } ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

struct EntradaNGE
{
   TipoEntNGE tipo ;   // objeto, transformacion, material
   union
   {  Objeto3D * objeto ;  // ptr. a un objeto (no propietario)
      Matriz4f * matriz ;  // ptr. a matriz 4x4 transf. (propietario)
      Material * material ; // ptr. a material (no propietario)
   } ;

   // constructores (uno por tipo)
   EntradaNGE( Objeto3D * pObjeto ) ;      // (copia solo puntero)
   EntradaNGE( const Matriz4f & pMatriz ); // (crea copia en el heap)
   EntradaNGE( Material * pMaterial ) ;    // (copia solo puntero)
   ~EntradaNGE() ;
} ;

// *********************************************************************
// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una lista de entradas

class NodoGrafoEscena : public Objeto3D
{
   protected:
     std::vector<EntradaNGE> entradas;
     Tupla3f color;

   public:

     NodoGrafoEscena() ;

     // visualiza usando OpenGL
     virtual void visualizarGL( ContextoVis & cv ) ;
     virtual void fijarColorNodo(const Tupla3f& color) ;

     // calcular y obtener la caja englobante
     //virtual CajaEngf cajaEnglobante() ;

     // añadir una entrada al final, hace copia de la entrada
     // devuelve indice de la entrada dentro del vector de entradas
     unsigned agregar( const EntradaNGE & entrada );

     // construir una entrada y añadirla (al final)
     unsigned agregar( Objeto3D * pObjeto ); // objeto (copia solo puntero)
     unsigned agregar( const Matriz4f & pMatriz ); // matriz (copia objeto)
     unsigned agregar( Material * pMaterial ); // material (copia solo puntero)

     // devuelve el puntero a la matriz en la i-ésima entrada
     Matriz4f * leerPtrMatriz( unsigned iEnt );

     // método para buscar un objeto con un identificador
     virtual bool
     buscarObjeto(const int ident_busc, // identificador a buscar
                  const Matriz4f & mmodelado, // matriz de modelado
                  Objeto3D ** objeto, // (salida) puntero al puntero al objeto
                  Tupla3f & centro_wc); // (salida) centro del objeto en coordenadas del mundo)

     // si 'centro_calculado' es 'false', recalcula el centro usando los centros
     // de los hijos (el punto medio de la caja englobante de los centros de hijos)
     virtual void calcularCentroOC() ;
} ;

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos que tienen además un vector de
// parámetros

class NodoGrafoEscenaParam : public NodoGrafoEscena
{
   protected:
     std::vector<Parametro> parametros;

   public:
      // devuelve el número de parámetros
      unsigned numParametros();

      // devuelve un puntero al i-ésimo parámetro (i < numParametros())
      Parametro * leerPtrParametro( unsigned i ) ;

      // actualiza el objeto para ir al siguiente cuadro,
      // se usa cuando están activadas las animaciones, una vez antes de cada frame
      void siguienteCuadro();
} ;

// ---------------------------------------------------------------------
// clases para el grafo de escena de la P3

class MallaTendedor : public NodoGrafoEscena {
  protected:
    static constexpr unsigned num_tiras = 8;
    static constexpr float esp_tiras = 4.0;

    class Tira : public NodoGrafoEscena {
      public:
        static constexpr float longitud_tira = 35.0;
        static constexpr float grosor_tira = 0.5;
        Tira();
    };

    class TiraBorde : public NodoGrafoEscena {
      public:
        static constexpr float grosor_tira_borde = 5.0 * Tira::grosor_tira;
        TiraBorde();
    };

    class TiraBordeEsquina : public NodoGrafoEscena {
      public:
        TiraBordeEsquina();
    };

    friend class Armazon;

  public:
    MallaTendedor();
    static constexpr float longitud_tira();
    static constexpr float numero_tiras();
    static constexpr float espacio_tiras();
};

class Armazon : public NodoGrafoEscena {
  public:
    Armazon();
};

class Pata : public NodoGrafoEscena {
    unsigned indice_pata;
    static constexpr float angulo_inicial = 140.0;
  public:
    Pata();
    Matriz4f* matriz_pata();
};

class AlaTendedor : public NodoGrafoEscena {
    unsigned indice_ala;
    static constexpr float angulo_inicial = -10.0;
  public:
    AlaTendedor();
    Matriz4f* matriz_ala();
};

class TendedorMitad : public NodoGrafoEscena {
    Matriz4f* m_ala;
    Matriz4f* m_pata;
  public:
    TendedorMitad();
    Matriz4f* matriz_ala();
    Matriz4f* matriz_pata();
};

class Caja : public NodoGrafoEscena {
    std::vector<Matriz4f*> m_caja;

    class PlanoCaja : public NodoGrafoEscena {
        unsigned indice_plano;
      public:
        PlanoCaja();
        Matriz4f* matriz_plano();
    };

  public:
    Caja();
    std::vector<Matriz4f*> matrices_caja();
};

class Tendedor : public NodoGrafoEscenaParam
{
  public:
    Tendedor();
};

// ---------------------------------------------------------------------
// clases para el grafo de escena de la P4

class Lata : public NodoGrafoEscena {
  public:
    Lata();
};

class Peones : public NodoGrafoEscena {
  protected:
    class PeonNegro : public NodoGrafoEscena {
      public:
        PeonNegro();
    };

    class PeonBlanco : public NodoGrafoEscena {
      public:
        PeonBlanco();
    };

    class PeonMadera : public NodoGrafoEscena {
      public:
        PeonMadera();
    };

  public:
    Peones();
};

#endif // GRAFO_ESCENA_HPP

// *********************************************************************
