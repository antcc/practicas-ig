// *********************************************************************
// **
// ** Informática Gráfica
// ** Declaraciones públicas auxiliares, comunes a todas las prácticas
// **
// *********************************************************************

#ifndef IG_PRACTICAS_HPP
#define IG_PRACTICAS_HPP

#include <string>
#include "materiales.hpp"
#include "cauce.hpp"
#include "Parametro.hpp"

// --------------------------------------------------------------------
// declaraciones adelantadas de clases (útiles para punteros)

class ColFuentesLuz ;


// ---------------------------------------------------------------------
// tipo de datos enumerado para los modos de visualización:

typedef enum
{
   modoPuntos,
   modoAlambre,
   modoSolido,

   modoColorNodoPlano,
   modoMateriales,
   numModosVis
}
   ModosVis;


const std::string nombreModo[numModosVis] =
   {
      "modoPuntos",
      "modoAlambre",
      "modoSolido",

      "modoColorNodoPlano",
      "modoMateriales"
   } ;

// --------------------------------------------------------------------
// contexto de visualización

class ContextoVis
{
   public:

   ModosVis       modoVis        ;   // modo de visualización actual
   bool           dibuAlambreEsc ;   // dibujar alambre escalado en modos solido/ajedrez
   bool           usarVBOs  ;        // true-> usar modo diferido (VBOS), false->usar modo inmediato
   bool           usarShader ;       // true -> usar shader prog, false no usar shader prog
   bool           modoSeleccionFBO ; // true -> dibujando en modo selección con FBO invisible (puede ser el back-buffer)
   bool           modoSeleccionRB ;  // true -> dibujando en modo selección usando render buffer de opengl
   int            identAct ;         // identificador actual en modo seleccion (nunca -1, >=0), inicialmente 0 antes de raiz
   PilaMateriales pilaMateriales ;   // pila de materiales
   ColFuentesLuz * colFuentes ;      // colección de fuentes de luz activa

   ContextoVis()
   {
      modoVis          = modoAlambre ;  // poner alambre por defecto
      dibuAlambreEsc   = true ;
      usarVBOs         = false ;
      usarShader       = false ;
      modoSeleccionRB  = false ;
      modoSeleccionFBO = false ;
      colFuentes       = nullptr ;
   }

};
// exportar 'redibujar_ventana' para que sea visible desde las prácticas (??)
extern bool redibujar_ventana ;

// funciones que deben ser llamadas desde las prácticas
void FijarFuncDesocupado( void (*func_desocupado_nueva)(void) ) ;
void FGE_Redibujado();
void FGE_Desocupado() ;
void FijarColorIdent( const int ident ) ; // 0 ≤ ident < 2
int  LeerIdentEnPixel( int xpix, int ypix );






#endif
