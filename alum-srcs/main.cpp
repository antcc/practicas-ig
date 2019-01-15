// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Carlos Ureña Almagro
// **
// ** Función 'main', inicialización y gestores de eventos
// **
// *********************************************************************

// includes de C/C++

#include <cctype>   // toupper
#include <string>   // std::string
#include <iostream> // std::cout
#include <fstream>  // ifstream
#include <cmath>    // fabs
#include <chrono>   // función 'now', tipos 'time_point' y 'duration'

// includes en ../include
#include "aux.hpp"  // include cabeceras de opengl / glut / glut / glew
#include "CamaraInter.hpp"

// includes de archivos en el directorio de trabajo (de las prácticas)
#include "practicas.hpp"
#include "practica1.hpp"
#include "practica2.hpp"
#include "practica3.hpp"
#include "practica4.hpp"
#include "practica5.hpp"

// evita la necesidad de escribir std::
using namespace std ;

// *********************************************************************
// **
// ** Variables y constantes globales (contienen el estado de la aplicación)
// **
// *********************************************************************

constexpr int
   numPracticas      = 5 ;       // número total de prácticas
int
   ventana_tam_x     = 1024,     // ancho inicial y actual de la ventana, en pixels
   ventana_tam_y     = 1024,     // alto inicial actual de la ventana, en pixels
   practicaActual    = 1 ,       // practica actual (cambiable por teclado) (1,2,3,4 o 5)
   mouse_pos_factor  = 1 ,       // factor de conversión para displays "retina" en macOS
   x_ant_mabd,                   // coord. de ratón X anterior en modo arrastrar con botón derecho pulsado
   y_ant_mabd ;                  // idem Y
const float
   cam_d_min          = 0.3,     // mínimo valor de 'cam_d'
   cam_d_incre_tecla  = 0.1,     // incremento de 'cam_d' por teclado
   cam_d_incre_scroll = 0.1,     // incremento de 'cam_d' por rueda del ratón
   cam_ab_incre_tecla = 3.0,     // incremento de ángulos por teclado
   cam_ab_incre_raton = 1.0 ;    // incremento de ángulos con el ratón (por pixel)
float
   cam_a             = 0.0,      // estado actual de la cámara: ángulo 'a'
   cam_b             = 0.0,      // estado actual de la cámara: ángulo 'b'
   cam_d             = 4.0;      // estado actual de la cámara: distancia del observador al origen
bool
   redibujar_ventana   = true ,  // usado para indicar que hay que redibujar la ventana antes de procesar los siguientes eventos
   terminar_programa   = false , // puesto a true para salir
   buffer_accum        = true ,  // true solo si hay buffer de acumulación de opengl para hacer antialiasing
   modo_arrastrar_bder = false ; // modo arrastrar con boton derecho pulsado
GLFWwindow *
   glfw_window       = nullptr ; // puntero a la ventana GLFW
ContextoVis
   contextoVis ;                 // contexto de visualización actual (incluye modo de visualización)
ShaderProg * shaders;             // Shaders a usar
// puntero a función que se ejecuta cuando no hay eventos pendientes
// (si es null no se hace nada)
void (*func_desocupado_actual)(void) = nullptr ;


// *********************************************************************
// **
// ** Funciones auxiliares:
// **
// *********************************************************************

// ----------------------------------------------------------------------------
// función que gestiona un error en GLFW, se fija con 'glfwSetErrorCallback'

void ErrorGLFW( int codigo, const char * msg )
{
   using namespace std ;
   cout << "Error de GLFW: " << msg << endl
        << "(programa abortado)" << endl ;
   exit(1);
}


// ---------------------------------------------------------------------
// asigna el color de fondo actual a todos los pixels de la ventana

void LimpiarVentana()
{
   glClearColor( 0.8, 0.8, 0.8, 1.0 );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

// ---------------------------------------------------------------------
// dibuja los objetos de la escena

void DibujarObjetos()
{
   using namespace std ;
   using namespace chrono ;

   switch( practicaActual )
   {
      case 1 :
         P1_DibujarObjetos( contextoVis  ) ; // definido en 'practica1.hpp'
         break ;
      case 2 :
         P2_DibujarObjetos( contextoVis  ) ; // definido en 'practica2.hpp'
         break ;
      case 3 :
         P3_DibujarObjetos( contextoVis  ) ; // definido en 'practica3.hpp'
         break ;
      case 4 :
         P4_DibujarObjetos( contextoVis  ) ; // definido en 'practica4.hpp'
         break ;
      case 5 :
         P5_DibujarObjetos( contextoVis  ) ; // definido en 'practica5.hpp'
         break ;
      default :
         cout << "El valor de 'practicaActual' (" << practicaActual  << ") es incorrecto" << endl ;
         break ;
   }
}

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// fijar las matrices modelview y projection de opengl
void FijarMVPOpenGL()
{
   const Matriz4f
      matrizVista = MAT_Traslacion( Tupla3f( 0.0, 0.0, -cam_d ) ) *
                    MAT_Rotacion( cam_b, 1.0,0.0,0.0 )*MAT_Rotacion( -cam_a, 0.0,1.0,0.0 ),
      matrizProye = MAT_Perspectiva( 70.0, float(ventana_tam_y)/float(ventana_tam_x), 0.1, cam_d+20.0 );

   glViewport( 0,0, ventana_tam_x, ventana_tam_y );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glMultMatrixf( matrizProye );

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glMultMatrixf( matrizVista );
}
// -----------------------------------------------------------------------------
// órdenes de OpenGL que fijan la camara y visualizan la escena

void DibujarEscena()
{
   if ( practicaActual == 5 )
      P5_FijarMVPOpenGL( ventana_tam_x, ventana_tam_y );
   else
      FijarMVPOpenGL();


   LimpiarVentana();
   DibujarEjesSolido() ;
   DibujarObjetos();
}

// *********************************************************************
// **
// ** Funciones gestoras de eventos
// **
// *********************************************************************

// función que visualiza un frame o cuadro

void VisualizarFrame()
{
   using namespace std ;
   using namespace chrono ;

   // hacer que la ventana GLFW sea la ventana actual
   glfwMakeContextCurrent( glfw_window );

   // Activar shader (NO ESTÁ PROGRAMADO, si lo activamos no funcionan las luces)
   if (contextoVis.usarShader)
     shaders->activar();

   DibujarEscena();  // ordenes OpenGL para dibujar la escena correspondiente a la práctica actual

   // visualizar en pantalla el buffer trasero (donde se han dibujado las primitivas)
   glfwSwapBuffers( glfw_window );
}

// ---------------------------------------------------------------------
// F.G. del evento de cambio de tamaño de la ventana

void FGE_CambioTamano( GLFWwindow* window, int nuevoAncho, int nuevoAlto )
{
   // leer tamaño del framebuffer (coincide con 'nuevoAncho' y 'nuevoAlto'
   // normalmente, excepto quizás en las pantalla retina de Apple)

   int fbx, fby ;
   glfwGetFramebufferSize( glfw_window, &fbx, &fby );

   if ( fbx > 0 && fby > 0 )
   {
      ventana_tam_x  = fbx ;
      ventana_tam_y  = fby ;
   }
   else // si glfwGetFrameBufferSize no devuelve valores no nulos, usar parámetros
   {
      ventana_tam_x = nuevoAncho ;
      ventana_tam_y = nuevoAlto ;
   }
   // forzar un nuevo evento de redibujado, para actualizar ventana
   redibujar_ventana = true ;
}

// ----------------------------------------------------------------------
// Cambia la función que se ejecuta cuando no hay nada que hacer
// (puede ser 'nullptr' para desactivar la función desocupado)

void FijarFuncDesocupado( void (*func_desocupado_nueva)(void) )
{
   func_desocupado_actual = func_desocupado_nueva ;
}

// ---------------------------------------------------------------------
// F.G. del evento de pulsación de la tecla normal
//
// parámetros:
//       tecla: carácter corresondiente a la tecla (minúscula)
//       x_raton, y_raton : posición del ratón al pulsar
// se establece como gestor con 'glfwSetCharCallback'

void FGE_PulsarTeclaCaracter( GLFWwindow* window, unsigned int codepoint )
{
   bool redibujar = true ; // true si al acabar de procesar el evento resulta que es necesario redibujar

   const char ch = char(codepoint);
   //cout << "FGE_PulsarTeclaCaracter( '" << ch << "' )" << endl ;

   unsigned int tecla = codepoint ;

   if ( practicaActual == 5 )  // las teclas normales de la 5 tienen preferencia sobre las del main.....
   if ( P5_FGE_PulsarTeclaCaracter( tecla  ) )
   {
      redibujar_ventana = true ;
      return ;
   }

   switch (toupper(ch))
   {
      case 'Q' :
      case 27  :
         exit( 0 );
         break ;
      case 'P' :
         practicaActual = (practicaActual % numPracticas) +1 ;
         cout << "Práctica actual cambiada a: " << practicaActual << endl << flush ;
         if ( practicaActual == 3 )
            FijarFuncDesocupado( FGE_Desocupado );
         break ;
      case '+' :
         cam_d = cam_d_min + (cam_d-cam_d_min)/(1.0+cam_d_incre_scroll) ;
         break;
      case '-' :
         cam_d = cam_d_min + (cam_d-cam_d_min)*(1.0+cam_d_incre_scroll) ;
         break;
      case 'M' :
         contextoVis.modoVis = ModosVis((int(contextoVis.modoVis)+1) % numModosVis) ;
         cout << "modo de visualización cambiado a: '" << nombreModo[contextoVis.modoVis] << "'" << endl << flush ;
         break ;
      case 'V' :
         if (contextoVis.modoVis == modoIluminacionPlano) {
          cout << "este modo de visualización no admite modo diferido." << endl << flush;
         }
         else {
           contextoVis.usarVBOs = !contextoVis.usarVBOs;
           cout << "forma de envío cambiada a: '" << (contextoVis.usarVBOs ? "modo diferido" : "modo inmediato") << "'" << endl << flush;
         }
         break;

      default:
         redibujar = false ;
         switch( practicaActual )
         {
            case 1 :
               redibujar = P1_FGE_PulsarTeclaCaracter( tecla ) ; // true si es necesario redibujar
               break ;
            case 2 :
               redibujar = P2_FGE_PulsarTeclaCaracter( tecla ) ; // true si es necesario redibujar
               break ;
            case 3 :
               redibujar = P3_FGE_PulsarTeclaCaracter( tecla ) ; // true si es necesario redibujar
               break ;
            case 4 :
               redibujar = P4_FGE_PulsarTeclaCaracter( tecla ) ; // true si es necesario redibujar
               break ;
            case 5 :
               redibujar = P5_FGE_PulsarTeclaCaracter( tecla ) ; // true si es necesario redibujar
               break ;
            default :
               cout << "numero de práctica incorrecto ("<< practicaActual <<")" << endl << flush ;
               break ;
         }
         break ;
   }

   // si se ha cambiado algo, forzar evento de redibujado
   if (redibujar)
      redibujar_ventana = true ; // REVISAR
}

// ---------------------------------------------------------------------
// F.G. del evento de pulsación de una tecla que no sea de tipo carácter


void FGE_PulsarTeclaEspecial( GLFWwindow* window, int key, int scancode, int action, int mods )
{
   // solo estamos interesados en el evento de levantar una tecla, ignoramos pulsar
   if ( action == GLFW_PRESS )
      return ;

   // si la tecla tiene 'nombre', entonces no la consideramos tecla especial
   // (es una tecla tipo 'carácter', el evento lo gestiona la otra f.g.e, la de tecla tipo carácter)
  // const char * keyname = glfwGetKeyName( key, scancode ) ;
   //if ( keyname != nullptr )
   //  return ;

   bool redisp = false ;


   if ( practicaActual == 5 )
      redisp = P5_FGE_PulsarTeclaEspecial( key );

   if ( ! redisp )
   {
      redisp = true ;
      switch ( key )
      {
      case GLFW_KEY_ESCAPE:
         terminar_programa = true ;
         break ;
      case GLFW_KEY_LEFT:
         cam_a += cam_ab_incre_tecla ;
         break;
      case GLFW_KEY_RIGHT:
         cam_a += -cam_ab_incre_tecla ;
         break;
      case GLFW_KEY_UP:
         cam_b += cam_ab_incre_tecla ;
         break;
      case GLFW_KEY_DOWN:
         cam_b += -cam_ab_incre_tecla ;
         break;
      case GLFW_KEY_PAGE_UP:
         cam_d += cam_d_incre_tecla ;
         break;
      case GLFW_KEY_PAGE_DOWN:
         cam_d = std::max( cam_d_min, cam_d-cam_d_incre_tecla );
         break;
      default:
         redisp = false ;
         break ;
	   }
   }

   // si se ha cambiado algo, forzar evento de redibujado
   if ( redisp )
      redibujar_ventana = true ;
}
// --------------------------------------------------------------------
// función que se invoca desde el bucle principal cuando las animaciones
// están activadas y no hay eventos pendientes de procesar
// (se usa en la práctica 3 para implementar las animaciones)

void FGE_Desocupado()
{
   bool desactivar = true ;
   if ( practicaActual == 3 )
      desactivar = ! P3_FGE_Desocupado();
   if ( desactivar )
      FijarFuncDesocupado( nullptr );
}

// ---------------------------------------------------------------------
// función que se invoca al inicio de mover el botón en modo arrastrar
// (movimientos del ratón con el botón derecho pulsado)

void InicioModoArrastrar( int x, int y )
{
   // activar modo arrastrar
   modo_arrastrar_bder = true ;
   // registrar la última posición del ratón
   x_ant_mabd = x ;
   y_ant_mabd = y ;
}
// ---------------------------------------------------------------------
// función que se  llama al subir el botón derecho del ratón, al acabar
// el modo arrastrar (movimientos del ratón con el botón derecho pulsado)

void FinModoArrastrar()
{
   // desactivar el modo arrastrar
   modo_arrastrar_bder = false ;
}
// ---------------------------------------------------------------------
// función que se llama durante el modo arrastrar
// (movimientos del ratón con el botón derecho pulsado)

void RatonArrastradoHasta( int x, int y )
{
   // registrar el desplazamiento en pixels en X y en Y
   const int dx = x - x_ant_mabd ,
             dy = y - y_ant_mabd ;

   cam_a += -dx*cam_ab_incre_raton ;
   cam_b +=  dy*cam_ab_incre_raton ;

   // registrar la última posición del ratón
   x_ant_mabd = x ;
   y_ant_mabd = y ;

}
// --------------------------------------------------------------------

// función gestora del evento de hacer scroll
// (se registra con {\bf glfwSetScrollCallback}
//

void FGE_Scroll( GLFWwindow* window, double xoffset, double yoffset  )
{
   // si hay poco movimiento vertical, ignorar el evento
   if ( fabs( yoffset) < 0.05 )
      return ;

   const int direction = 0.0 < yoffset ? +1 : -1 ;

   // este evento se gestiona en la práctica 5 si está activada
   if ( practicaActual == 5 )
   {
      if ( P5_FGE_Scroll( direction ))
         redibujar_ventana = true ;
      return ;
   }

   // en el resto de prácticas, supone cambiar zoom
   if ( direction == +1 )
      cam_d = cam_d_min + (cam_d-cam_d_min)*(1.0+cam_d_incre_scroll) ;
   else
      cam_d = cam_d_min + (cam_d-cam_d_min)/(1.0+cam_d_incre_scroll) ;

   redibujar_ventana = true ; // REVISAR
}
// --------------------------------------------------------------------
// función gestora del evento de pulsar/levantar tecla del ratón
// (se registra con {\bf glfwSetMouseButtonCallback})

void FGE_ClickRaton( GLFWwindow* window, int button, int action, int mods )
{
   // leer posicion del ratón
   double x_f,y_f ;

   // leer la posición del puntero de ratón en x,y (enteros)

   glfwGetCursorPos( window, &x_f, &y_f );
   const int x = int(x_f),
             y = int(y_f);

   if ( practicaActual == 5 )
   {
      if ( P5_FGE_ClickRaton( button, action, x, y ) )
         redibujar_ventana = true ;
      return ;
   }

   // si se pulsa el boton derecho, inicia modo arrastrar, si se levanta, fin modo arrastrar
   // (si no es el botón derecho, nada)
   if ( button == GLFW_MOUSE_BUTTON_RIGHT)
   {
      if ( action == GLFW_PRESS )
         InicioModoArrastrar( x, y );
      else
         FinModoArrastrar();
      redibujar_ventana = true ;
   }
}

// --------------------------------------------------------------------
// función gestora del evento de ratón movido a una nueva posición
// (se registra con {\bf glfwSetCursorPosCallback})

void FGE_RatonMovido( GLFWwindow* window, double xpos, double ypos )
{

   // ignorar evento si no está pulsado el botón derecho
   if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) != GLFW_PRESS )
      return ;

   // invocar la función de la práctica 5
   if ( practicaActual == 5 )
   {
      if ( P5_FGE_RatonMovidoPulsado( int(xpos), int(ypos) ) )
         redibujar_ventana = true ;
      return ;
   }

   // para el resto de prácticas, si se está en modo arrastrar, hacer lo que corresponda
   if ( modo_arrastrar_bder )
   {
      RatonArrastradoHasta( int(xpos), int(ypos) );
      redibujar_ventana = true ;
   }
}

// *********************************************************************
// **
// ** Funciones de inicialización
// **
// *********************************************************************

// inicialización de GLFW: creación de la ventana, designar FGEs

void Inicializa_GLFW( int argc, char * argv[] )
{

   cout << "Inicializa GLFW, comienza..." << endl ;

   // Initialize the library
   if ( ! glfwInit() )
   {
      cout << "Imposible inicializar librería GLFW. Programa terminado." << endl ;
      exit(1) ;
   }

   // especificar que función se llamará ante un error de GLFW
   glfwSetErrorCallback( ErrorGLFW );

   // crear la ventana
   glfw_window = glfwCreateWindow( ventana_tam_x, ventana_tam_y,
                    "Practicas IG GIM (18-19)", nullptr, nullptr );

   if ( glfw_window == nullptr )
   {
      cout << "Error: imposible crear ventana. Programa terminado." << endl ;
      glfwTerminate();
      exit(1);
   }
   // establecer el contexto de la ventana como el contexto actual
   glfwMakeContextCurrent( glfw_window );

   // calcular el tamaño inicial de la ventana (ventana_tam_x/_y)
   int fbx, fby, wx, wy ;
   glfwGetWindowSize( glfw_window, &wx, &wy );
   glfwGetFramebufferSize( glfw_window, &fbx, &fby );

   if ( fbx > 0 && fby > 0 ) // REVISAR, parece que en ubuntu esto devuelve: 0,0 en fbx,fby ¿?
   {
      ventana_tam_x  = fbx ;
      ventana_tam_y  = fby ;
      mouse_pos_factor = fbx/wx ;  // en displays macos retina, esto es 2
   }
   else
   {
      ventana_tam_x = wx ;
      ventana_tam_y = wy ;
      mouse_pos_factor = 1 ;
   }

   // definir las diversas funciones gestoras de eventos que GLFW debe invocar
   // (lo hace en glfwPollEvents o en glfwWaitEvents)

   glfwSetWindowSizeCallback ( glfw_window, FGE_CambioTamano );
   glfwSetKeyCallback        ( glfw_window, FGE_PulsarTeclaEspecial );
   glfwSetCharCallback       ( glfw_window, FGE_PulsarTeclaCaracter );
   glfwSetMouseButtonCallback( glfw_window, FGE_ClickRaton );
   glfwSetCursorPosCallback  ( glfw_window, FGE_RatonMovido );
   glfwSetScrollCallback     ( glfw_window, FGE_Scroll );
}

// ---------------------------------------------------------------------
// Inicialización de las variables globales del programa

void Inicializa_Vars( )
{
   // inicializar práctica actual
   practicaActual = 5 ;
   contextoVis.modoVis = modoAlambre;
}

// ---------------------------------------------------------------------
// inicialización de OpenGL

void Inicializa_OpenGL( )
{
   // borrar posibles errores anteriores
   CError();

   // habilitar test de comparación de profundidades para 3D (y 2D)
   // es necesario, no está habilitado por defecto:
   // https://www.opengl.org/wiki/Depth_Buffer
   glEnable( GL_DEPTH_TEST );

   // deshabilitar filtro de triangulos por su orientación:
   glDisable( GL_CULL_FACE );

   // establecer color de fondo: (1,1,1) (blanco)
   glClearColor( 1.0, 1.0, 1.0, 1.0 ) ;

   // establecer color inicial para todas las primitivas
   glColor3f( 0.7, 0.2, 0.4 ) ;

   // establecer ancho de línea
   glLineWidth( 1.2 );

   // establecer tamaño de los puntos
   glPointSize( 1.7 );

   // establecer modo de visualización de prim.
   // (las tres posibilidades son: GL_POINT, GL_LINE, GL_FILL)
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

   // imprimir datos del hardware y la implementación de OpenGL
   using namespace std ;
   cout  << "Datos de versión e implementación de OpenGL" << endl
         << "  implementación de : " << glGetString(GL_VENDOR)  << endl
         << "  hardware          : " << glGetString(GL_RENDERER) << endl
         << "  version de OpenGL : " << glGetString(GL_VERSION) << endl
         << "  version de GLSL   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl
         << flush ;

   Inicializa_GLEW();


   // Habilitar shaders
   shaders = new SimpleSP;

   // ya está
   CError();
}

// ---------------------------------------------------------------------
// Código de inicialización (llama a los dos anteriores, entre otros)

void Inicializar( int argc, char *argv[] )
{
   // inicializa las variables del programa
   Inicializa_Vars() ;

   // glut (crea la ventana)
   Inicializa_GLFW( argc, argv ) ;

   // opengl: define proyección y atributos iniciales
   Inicializa_OpenGL() ;

   // inicializar práctica 1.
   P1_Inicializar(  ) ;

   // inicializar práctica 2.
   P2_Inicializar(argc, argv) ;

   // inicializar la práctica 3
   P3_Inicializar(  );

   // inicializar la práctica 4
   P4_Inicializar(  );

   // inicializar la práctica 5
   P5_Inicializar( ventana_tam_x, ventana_tam_y );
}

// ---------------------------------------------------------------------
// bucle principal  de gestion de eventos GLFW

void BucleEventosGLFW()
{
   // ejecutar el bucle de gestión de eventos hasta el final de la aplicación

   terminar_programa = false ;

   while ( ! terminar_programa  )
   {
      if ( redibujar_ventana )   // si ha cambiado algo:
      {
         VisualizarFrame();            // dibujar la escena
         redibujar_ventana = false ;   // evitar que se redibuje continuamente
      }
      if ( func_desocupado_actual == nullptr ) // si no hay definida la función 'desocupado'
         glfwWaitEvents();                     //    esperar hasta que haya un evento y llamar a la función correspondiente, si está definida
      else                                     // si hay una función 'desocupado' activa
      {  glfwPollEvents();                     //    procesar todos los eventos pendientes, y llamar a la función correspondiente, si está definida
         if ( ! redibujar_ventana )            //    si no es necesario redibujar la ventana
            func_desocupado_actual();          //       ejecutar la función 'idle' actual (ya no hay eventos pendientes)
      }
      terminar_programa = terminar_programa || glfwWindowShouldClose( glfw_window ) ;
   }

   glfwTerminate();
}

// *********************************************************************
// **
// ** Función principal
// **
// *********************************************************************

int main( int argc, char *argv[] )
{
   // incializar las variables del programa
   Inicializar( argc, argv ) ;

   // ejecutar el bucle principal de gestión de eventos de GLFW
   BucleEventosGLFW();

   // ya está
   return 0;
}
