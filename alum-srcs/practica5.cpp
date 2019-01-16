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
static const float DELTA = 5.0;
// viewport actual (se recalcula al inicializar y al fijar las matrices)
static Viewport viewport;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;
// número de cámaras
static constexpr int numCamaras = 4;
// vector de cámaras con vista como mínimo de alzado, planta y perfil
static CamaraInteractiva * camaras[numCamaras] = {nullptr, nullptr, nullptr, nullptr};
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
   objetos5[0] = new EscenaP5;
   viewport = Viewport(0, 0, vp_ancho, vp_alto);
   float ratio = (float) vp_alto / vp_ancho;
   // Alzado
   camaras[0] = new CamaraInteractiva (true, ratio, 0, 0,
                                       {0,0,0}, true, 80.0, 1.8);
   // Perfil
   camaras[1] = new CamaraInteractiva (true, ratio, 90, 0,
                                       {0,0,0}, true, 80.0, 2.0);
   // Planta
   camaras[2] = new CamaraInteractiva (true, ratio, 0, 90,
                                       {0,0,0}, true, 80.0, 4.0);
   // Alzado ortográfica
   camaras[3] = new CamaraInteractiva (true, ratio, 0, 0,
                                       {0,0,0}, false, 80.0, 3.2);
   cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{
   viewport = Viewport(0, 0, vp_ancho, vp_alto);
   viewport.fijarViewport();

   camaras[camaraActiva]->ratio_yx_vp = viewport.ratio_yx;
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
   bool result = true;
   bool examinar;

   switch (toupper(tecla))
   {
      case 'C':
        camaraActiva = (camaraActiva + 1) % numCamaras;
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
        cout << "práctica 5: desplazamiento en el eje Z (acercar)" << endl;
        break;

      case '-':
        camaras[camaraActiva]->desplaZ(-DELTA);
        cout << "práctica 5: desplazamiento en el eje Z (alejar)" << endl;
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
// la cámara activa mirando a dicho objeto en modo examinar.

void P5_ClickIzquierdo( int x, int y )
{
  // Visualizar en modo selección

  ContextoVis cv;
  cv.modoSeleccionFBO = true;

  glClearColor(0, 0, 0, 1); // color de fondo
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpiar pantalla

  objetos5[0]->visualizarGL(cv);

  // Leer color del pixel pinchado

  unsigned ident = LeerIdentEnPixel(x, y);

  if (ident == 0) {
   cout << "práctica 5: no se ha seleccionado ningún objeto" << endl;
   return;
  }

  // Si no es 0, buscamos el objeto y ponemos la cámara en modo examinar

  Tupla3f centro_wc(0.0, 0.0, 0.0);
  Matriz4f m = MAT_Ident();
  Objeto3D * objeto = nullptr;

  if (objetos5[0]->buscarObjeto(ident, m, &objeto, centro_wc)) {
    camaras[camaraActiva]->modoExaminar(centro_wc);
    cout << "práctica 5: seleccionado objeto " << objeto->leerNombre()
         << " con centro " << centro_wc << endl;
  }

  else {
    cout << "práctica 5: no se ha encontrado el objeto seleccionado" << endl;
  }
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
  if (ident >= 0) {
    const unsigned char
      byteR = ident % 0x100U,
      byteG = (ident / 0x100U) % 0x100U,
      byteB = (ident / 0x10000U) % 0x100U;

    glColor3ub(byteR, byteG, byteB);
  }
}
//---------------

int LeerIdentEnPixel( int xpix, int ypix )
{
  unsigned char bytes[3];
  // leer los 3 bytes del frame-buffer (pixel = 1x1)
  glReadPixels(xpix, ypix, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *) bytes);
  // reconstruir el indentificador y devolverlo:
  return bytes[0] + (0x100U * bytes[1]) + (0x10000U * bytes[2]) ;
}
//---------------
