// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (declaraciones públicas)
// **
// *********************************************************************

#ifndef IG_PRACTICA5_HPP
#define IG_PRACTICA5_HPP

void P5_Inicializar( int vp_ancho, int vp_alto );
void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto );
void P5_DibujarObjetos( ContextoVis & cv ) ;

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla ) ;
bool P5_FGE_PulsarTeclaEspecial(  int tecla ) ;
bool P5_FGE_ClickRaton( int boton, int estado, int x, int y );
bool P5_FGE_RatonMovidoPulsado( int x, int y );
bool P5_FGE_Scroll( int direction );


#endif
