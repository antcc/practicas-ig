// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"
#include "CamaraInter.hpp"

using namespace std;

// Desplazamiento de la cámara actual en el eje Z
static const float DELTA = 2.0;
// viewport actual (se recalcula al inicializar y al fijar las matrices)
static Viewport viewport;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;
// número de cámaras
static constexpr int numCamaras = 3;
// vector de cámaras con vista como mínimo de alzado, planta y perfil
static CamaraInteractiva * camaras[numCamaras] = {nullptr, nullptr, nullptr};
// cámara activa
static int camaraActiva = 0;
// posiciones anteriores del ratón
static int xant,
           yant;
// grafo de escena
static constexpr int numObjetos5 = 1;
static NodoGrafoEscena * objetos5[numObjetos5] = {nullptr};
static ColFuentesLuz * cf5 = nullptr;

// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto )
{
   cout << "Creando objetos de la práctica 5 .... " << flush ;
   cf5 = new ColeccionFuentesP4;
   objetos5[0] = new Peones;
   viewport = Viewport(0, 0, vp_ancho, vp_alto);
   camaras[0] = new CamaraInteractiva (false, (float) vp_ancho / vp_alto, 0, 0,
                                       {0,0,0}, false, 90, 2);
   camaras[1] = new CamaraInteractiva (false, (float) vp_ancho / vp_alto, 0, 0,
                                       {0,0,0}, true, 135, 2);
   camaras[2] = new CamaraInteractiva (false, (float) vp_ancho / vp_alto, 0, 0,
                                       {0,0,0}, true, 225, 2);
   cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{
   viewport.ancho = vp_ancho;
   viewport.alto = vp_alto;
   camaras[camaraActiva]->ratio_yx_vp = (float) vp_alto / vp_ancho;
   camaras[camaraActiva]->calcularViewfrustum(); // recalcular m_proy

   camaras[camaraActiva]->activar();
}
// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv )
{
  // Se supone que la cámara ya está activada
  cf5->activar();
  if (objetos5[0] != nullptr)
    objetos5[0]->visualizarGL(cv);
}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla )
{
   bool result = true ;
   bool examinar;

   switch ( toupper( tecla ) )
   {
      case 'C':
        camaraActiva = (camaraActiva + 1) % numCamaras;
        camaras[camaraActiva]->activar(); // ?
        cout << "práctica 5: nueva cámara activa es " << camaraActiva << endl;

        break ;

      case 'V':
         examinar = camaras[camaraActiva]->examinar;
         if (examinar)
           camaras[camaraActiva]->modoPrimeraPersona();
         else
            camaras[camaraActiva]->modoExaminar();
         cout << "práctica 5: modo de la cámara " << camaraActiva
              << " cambiado a " << (examinar ? "primera persona" : "examinar") << endl;

         break ;

      case '+': // ¿Es al revés?
         camaras[camaraActiva]->desplaZ(DELTA);
         cout << "práctica 5: desplazamiento en el eje Z positivo" << endl;

         break;

      case '-':
        camaras[camaraActiva]->desplaZ(-DELTA);
        cout << "práctica 5: desplazamiento en el eje Z negativo" << endl;

         break;

      default:
         result = false ;
         break ;
	}

   return result ;
}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(  int tecla  )
{
   bool result = true ;

   switch ( tecla )
   {
      case GLFW_KEY_LEFT:
         camaras[camaraActiva]->moverHV(-DELTA, 0);
         cout << "práctica 5: desplazamiento hacia la izquierda" << endl;

         break;
      case GLFW_KEY_RIGHT:
        camaras[camaraActiva]->moverHV(DELTA, 0);
        cout << "práctica 5: desplazamiento hacia la derecha" << endl;

         break;
      case GLFW_KEY_UP:
         camaras[camaraActiva]->moverHV(0, DELTA);
         cout << "práctica 5: desplazamiento hacia arriba" << endl;

         break;
      case GLFW_KEY_DOWN:
         camaras[camaraActiva]->moverHV(0, -DELTA);
         cout << "práctica 5: desplazamiento hacia abajo" << endl;

         break;
      default:
         result = false ;
         break ;
	}

   return result ;
}
// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo.
// calcula objeto sobre el que se ha hecho el click, si hay alguno seleccionado pone
// la cámara activa mirando a dicho objeto en modo examinar y devuelve true,
// en otro caso devuelve false

void P5_ClickIzquierdo( int x, int y )
{
   // COMPLETAR: práctica 5: visualizar escena en modo selección y leer el color del pixel en (x,y)

   // 1. crear un 'contextovis' apropiado
   // .....

   // 2. visualizar en modo selección (sobre el backbuffer)
   // ....

   // 3. leer el color del pixel, si es 0 no se hace nada
   // .....

   // 4. buscar el objeto en el grafo de escena e informar del mismo
   // .....

}
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y )
{
   modo_arrastrar = true;
   xant = x;
   yant = y;
}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
   modo_arrastrar = false;
}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y )
{
   // mover cámara
   camaras[camaraActiva]->moverHV(x - xant, y - yant);

   // registrar última posición
   xant = x;
   yant = y;
}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
   //cout << "P5_FGE_ClickRaton" << endl ;
   if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
      P5_ClickIzquierdo( x, viewport.alto-y );
   else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_InicioModoArrastrar( x, viewport.alto-y );
   else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_FinModoArrastrar();
   else
      return false ;

   return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
   if ( modo_arrastrar )
   {
      P5_RatonArrastradoHasta( x, viewport.alto-y );
      return true ;
   }
   else
      return false ;
}
// ---------------------------------------------------------------------

// direction = +- 1
bool P5_FGE_Scroll( int direction )
{
  camaras[camaraActiva]->desplaZ(direction * DELTA);
  return true ;
}
// ---------------------------------------------------------------------

void FijarColorIdent( const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (glColor3ub)
   // .....

}
//---------------

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   // .....

}
//---------------
