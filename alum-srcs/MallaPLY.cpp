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

MallaPLY::MallaPLY(const std::string & nombre_arch)
  : MallaInd(string("malla leída del archivo '") + nombre_arch + "'" )
{
   vector<float> vertices;
   vector<int> caras;

   ply::read(nombre_arch.c_str(), vertices, caras);

   for (unsigned i = 0; i + 2 < vertices.size(); i += 3)
    tabla_vertices.push_back({vertices[i], vertices[i+1], vertices[i+2]});

   for (unsigned i = 0; i + 2 < caras.size(); i+= 3)
     tabla_caras.push_back({caras[i], caras[i+1], caras[i+2]});

   num_vertices = vertices.size() / 3;

   // Color
   setColorVertices();

   // Centro
   calcularCentroOC();
}

// *****************************************************************************
