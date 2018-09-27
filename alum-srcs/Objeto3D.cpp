// *********************************************************************
// **
// ** Informática Gráfica
// ** Definición de los métodos de Objeto3D
// **
// *********************************************************************

#include <iostream>
#include "Objeto3D.hpp"

using namespace std ;

// -----------------------------------------------------------------------------
Objeto3D::Objeto3D()
{
   ponerIdentificador( 0 );
   ponerNombre("objeto anónimo");
   ponerCentroOC( Tupla3f( 0.0, 0.0, 0.0 ) );
}

// -----------------------------------------------------------------------------
// devuelve el nombre del objeto
std::string Objeto3D::leerNombre()
{
   return nombre_obj ;
}

// -----------------------------------------------------------------------------
// poner nombre al objeto
void Objeto3D::ponerNombre( const std::string & nuevoNombre )
{
   nombre_obj = nuevoNombre ;
}
// -----------------------------------------------------------------------------
// devuelve el nombre del objeto
int Objeto3D::leerIdentificador()
{
   return identificador ;
}

// -----------------------------------------------------------------------------
// ponerle un identificador al objeto
void Objeto3D::ponerIdentificador( int nuevoIdent )
{
   identificador = nuevoIdent ;
}

// -----------------------------------------------------------------------------
// poner el centro del objeto:
void Objeto3D::ponerCentroOC( const Tupla3f & nuevoCentro )
{
   centro_oc = nuevoCentro ;
}

// -----------------------------------------------------------------------------
// leer el centro del objeto
Tupla3f Objeto3D::leerCentroOC()
{
   return centro_oc ;
}

// -----------------------------------------------------------------------------
// (re)calcular centro del objeto, si es necesario
void Objeto3D::calcularCentroOC()
{
   // por defecto no se hace nada: se asume que el centro está bien calculado
   // en el constructor. Solo se redefine este método en los Nodos de grafo de escena
}

// -----------------------------------------------------------------------------
// buscar un identificador (implementación por defecto para todos los Objeto3D)

bool Objeto3D::buscarObjeto
(
   const int        ident_busc,
   const Matriz4f & mmodelado,
   Objeto3D **      objeto,
   Tupla3f &        centro_wc
)
{
   assert( 0 < ident_busc );

   if ( identificador == ident_busc )
   {
      centro_wc = mmodelado*leerCentroOC();
      if ( objeto != nullptr )
         *objeto = this ;
      return true ;
   }
   else
      return false ;
}

// -----------------------------------------------------------------------------
// destructor
Objeto3D::~Objeto3D()
{
   using namespace std ;
   cout << "destruyendo objeto3D de nombre: " << nombre_obj << endl << flush ;
}
