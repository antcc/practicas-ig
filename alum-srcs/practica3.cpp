// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"



using namespace std ;

// COMPLETAR: práctica 3: declarar variables de la práctica 3 (static)
// .................




// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush ;
   // COMPLETAR: práctica 3: inicialización de objetos de la práctica 3
   // .................

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
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
         // COMPLETAR: práctica 3: activar siguiente objeto de la práctica
         //           (solo en el caso de que se incluyan varios objetos en la práctica)
         // ....

         break ;

      case 'A' :
         // COMPLETAR: práctica 3: activar o desactivar animaciones
         // ....

         break ;

      case 'G' :
         // COMPLETAR: práctica 3: activar siguiente parámetro
         // ....

         break ;

      case 'R' :
         // COMPLETAR: práctica 3: reset de animaciones
         // ....

         break ;

      case '>' :
         // COMPLETAR: práctica 3: acelerar o incrementar parámetro
         // ....

         break ;

      case '<' :
         // COMPLETAR: práctica 3: decelerar o decrementar parámetro
         // ....

         break ;
      default :
         break ;

   }
   return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv )
{
   // COMPLETAR: práctica 3: visualizar el objeto de la práctica 3
   // ....


}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
   // COMPLETAR: práctica 3: si las animaciones están activadas,
   // actualizar el objeto activo, forzar redibujado, devolver true.
   // si las animaciones están desactivadas, devolver false.
   // ....

}
