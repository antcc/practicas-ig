// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre(string("malla leída del archivo '") + nombre_arch + "'" );
   ply::read(nombre_arch, vertices, caras);

   // calcular la tabla de normales
   calcular_normales();
}

// *****************************************************************************
