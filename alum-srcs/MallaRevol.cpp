// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include "MallaRevol.hpp"

using namespace std ;

// *****************************************************************************

MallaRevol::MallaRevol(const string& nombre)
  : MallaInd(nombre)
{

}

// *****************************************************************************

MallaRevol::MallaRevol(const std::string & nombre_arch,
                       const unsigned nperfiles,
                       const bool crear_tapas,
                       const bool cerrar_malla)
  : MallaInd(string("malla por revolución del perfil en '"+ nombre_arch + "'" ))
{
  vector<float> vertices;
  vector<Tupla3f> perfil_original;

  ply::read_vertices(nombre_arch.c_str(), vertices);

  for (int i = 0; i < vertices.size(); i += 3)
    perfil_original.push_back({vertices[i], vertices[i+1], vertices[i+2]});

  inicializarMallaRevol(perfil_original, nperfiles, vertices.size() / 3, crear_tapas,
                        cerrar_malla);
}

// *****************************************************************************

void MallaRevol::inicializarMallaRevol(const std::vector<Tupla3f> & perfil_original,
                                       int nperfiles,
                                       int num_verts_per,
                                       bool crear_tapas,
                                       bool cerrar_malla)
{
  nper = nperfiles;
  nvp = num_verts_per;
  num_vertices = nper * nvp; // vértices totales

  // Construir vértices y caras
  crearMallaRevol(perfil_original, crear_tapas, cerrar_malla);

  // Color
  setColorVertices();

  // calcular la tabla de normales
  calcular_normales();
}

// *****************************************************************************

void MallaRevol::crearMallaRevol(const std::vector<Tupla3f> & perfil_original,
                                 bool crear_tapas,
                                 bool cerrar_malla)
{

}

// *****************************************************************************

Cilindro::Cilindro(const int num_verts_per,
                   const unsigned nperfiles,
                   const bool crear_tapas,
                   const bool cerrar_malla)
  : MallaRevol("malla por revolución de un cilindro")
{
  vector<Tupla3f> perfil_original;

  // -- Construir perfil --

  inicializarMallaRevol(perfil_original, nperfiles, num_verts_per, crear_tapas, cerrar_malla);
}

// *****************************************************************************

Cono::Cono(const int num_verts_per,
     const unsigned nperfiles,
     const bool crear_tapas,
     const bool cerrar_malla)
  : MallaRevol("malla por revolución de un cono")
{
  vector<Tupla3f> perfil_original;

  // -- Construir perfil --

  inicializarMallaRevol(perfil_original, nperfiles, num_verts_per, crear_tapas, cerrar_malla);
}

// *****************************************************************************

Esfera::Esfera(const int num_verts_per,
       const unsigned nperfiles,
       const bool crear_tapas,
       const bool cerrar_malla)
  : MallaRevol("malla por revolución de una esfera")
{
  vector<Tupla3f> perfil_original;

  // -- Construir perfil --

  inicializarMallaRevol(perfil_original, nperfiles, num_verts_per, crear_tapas, cerrar_malla);
}
