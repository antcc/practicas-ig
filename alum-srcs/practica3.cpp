// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 3  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"

using namespace std ;

static bool p3_animaciones_activas = false;
static int p3_grado_libertad_activo = 0;
static constexpr int numObjetos3 = 1;
static unsigned objetoActivo3 = 0 ;
static NodoGrafoEscenaParam * objetos3[numObjetos3] = {nullptr};


// ---------------------------------------------------------------------
// Función para implementar en la práctica 3 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush ;
   objetos3[0] = new C();
   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 3 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto (true si se ha procesado tecla)

   switch ( toupper( tecla ) )
   {
      case 'O' :
        objetoActivo3 = (objetoActivo3 + 1) % numObjetos3 ;
        cout << "práctica 3: nuevo objeto activo es " << objetoActivo3 ;
        if ( objetos3[objetoActivo3] != nullptr )
           cout << " (" << objetos3[objetoActivo3]->leerNombre() << ")." << endl ;
        else
           cout << " (objeto no creado)" << endl ;
        res = true ;

        break ;

      case 'A' :
         p3_animaciones_activas = !p3_animaciones_activas;
         if (p3_animaciones_activas)
           FijarFuncDesocupado(FGE_Desocupado);  // TODO: ¿es necesario?

         cout << "práctica 3: animaciones "
              << (p3_animaciones_activas ? "activadas" : "desactivadas") << endl;
         res = true;

         break ;

      case 'G' :
         if ( objetos3[objetoActivo3] != nullptr ) {
           p3_grado_libertad_activo = (p3_grado_libertad_activo + 1) % objetos3[objetoActivo3]->numParametros();
           cout << "prática 3: nuevo grado de libertad activo es " << p3_grado_libertad_activo
                << endl;
           res = true;
         }

         break ;

      case 'R' :
         p3_animaciones_activas = false;

         if (objetos3[objetoActivo3] != nullptr)
           for (int i = 0; i < objetos3[objetoActivo3]->numParametros(); i++)
             objetos3[objetoActivo3]->leerPtrParametro(i)->reset();

         cout << "práctica 3: parámetros reseteados y animaciones desactivadas." << endl;
         res = true;

         break ;

      case '>' : //TODO
         if (objetos3[objetoActivo3] != nullptr) {
           if (p3_animaciones_activas) {

           }

           else {

           }

           res = true;
         }
         break ;

      case '<' : //TODO
         if (objetos3[objetoActivo3] != nullptr) {
           if (p3_animaciones_activas) {

           }

           else {

           }

           res = true;
         }

         break ;

      default :
         break ;
   }

   return res ;
}

// ---------------------------------------------------------------------

void P3_DibujarObjetos( ContextoVis & cv )
{
  objetos3[objetoActivo3]->visualizarGL(cv);
}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
  if (!p3_animaciones_activas)
    return false;

  //TODO: actualizar el objeto activo

  redibujar_ventana = true;

  return true;
}
