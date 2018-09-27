// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaPLY
// **
// *********************************************************************

#ifndef IG_MALLAPLY_HPP
#define IG_MALLAPLY_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaPLY : public MallaInd
{

   public:
      // crea una malla de revolucion
      MallaPLY( const std::string & nombre_arch ) ;

} ;

#endif
