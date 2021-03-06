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
// Funciones auxiliares

vector<Tupla3f> rotarPerfil(const Matriz4f& m,
                            const std::vector<Tupla3f>& perfil)
{
  vector<Tupla3f> rotado;
  for(auto vert : perfil)
    rotado.push_back(m * vert);
  return rotado;
}

// *****************************************************************************
// Implementación de métodos

MallaRevol::MallaRevol(const string& nombre)
  : MallaInd(nombre)
{

}

// *****************************************************************************

MallaRevol::MallaRevol(const std::string & nombre_arch,
                       const unsigned nperfiles,
                       const bool crear_tapas,
                       const bool cerrar_malla,
                       const bool crear_texturas)
  : MallaInd(string("malla por revolución del perfil en '"+ nombre_arch + "'" ))
{
  vector<float> vertices;
  vector<Tupla3f> perfil_original;

  ply::read_vertices(nombre_arch.c_str(), vertices);

  for (unsigned i = 0; i + 2 < vertices.size(); i += 3)
    perfil_original.push_back({vertices[i], vertices[i+1], vertices[i+2]});

  inicializarMallaRevol(perfil_original, nperfiles, vertices.size() / 3, crear_tapas,
                        cerrar_malla, crear_texturas);
}

// *****************************************************************************

void MallaRevol::inicializarMallaRevol(const std::vector<Tupla3f> & perfil_original,
                                       unsigned nperfiles,
                                       unsigned num_verts_per,
                                       bool crear_tapas,
                                       bool cerrar_malla,
                                       bool crear_texturas)
{
  nper = nperfiles;
  nvp = num_verts_per;
  num_vertices = nper * nvp; // vértices totales

  // Construir vértices y caras
  crearMallaRevol(perfil_original, crear_tapas, cerrar_malla, crear_texturas);

  // Color
  setColorVertices();
}

// *****************************************************************************

void MallaRevol::crearMallaRevol(const std::vector<Tupla3f> & perfil_original,
                                 bool crear_tapas,
                                 bool cerrar_malla,
                                 bool crear_texturas)
{

  unsigned i, j;
  vector<float> d;
  float alpha = cerrar_malla ? 360.0 / nper : 360.0 / (nper -1);
  Matriz4f m = MAT_Rotacion(alpha, 0, 1, 0);
  vector<Tupla3f> aux = perfil_original;
  tabla_vertices.insert(tabla_vertices.end(), aux.begin(), aux.end());

  // Añadir vértices
  for (i = 0; i < nper - 1; i++) {
    aux = rotarPerfil(m, aux);
    tabla_vertices.insert(tabla_vertices.end(), aux.begin(), aux.end());

    // Añadir caras
    for (j = 0; j < nvp - 1; j++) {
      int k1 = i * nvp + j;
      int k2 = i * nvp + j + 1;
      int k3 = (i+1) * nvp + j + 1;
      int k4 = (i+1) * nvp + j;

      tabla_caras.push_back({k4, k3, k2});
      tabla_caras.push_back({k4, k2, k1});
    }
  }

  if (cerrar_malla) {
    // Añadir caras (i = nper - 1)
    for (j = 0; j < nvp - 1; j++) {
      int k1 = i * nvp + j;
      int k2 = i * nvp + j + 1;
      int k3 = j + 1; // ((i+1) % nper) * nvp + j + 1
      int k4 = j; // ((i+1) % nper) * nvp + j

      tabla_caras.push_back({k4, k3, k2});
      tabla_caras.push_back({k4, k2, k1});
    }
  }

  // Coordenadas de textura
  if (crear_texturas && !cerrar_malla) {
    d.push_back(0.0);
    for (unsigned i = 1; i < nvp; i++)
      d.push_back(d[i-1] + sqrt((perfil_original[i] - perfil_original[i-1]).lengthSq()));

    for (unsigned i = 0; i < nper; i++)
      for (unsigned j = 0; j < nvp; j++)
        texturas.push_back({(float) i / (nper - 1), 1 - (float) d[j] / d[nvp - 1]});
  }

  // Crear tapas
  if (crear_tapas) {

    // Cara inferior
    auto primer_ver = tabla_vertices[0];
    if (primer_ver(X) != 0) {
      tabla_vertices.push_back({0.0, primer_ver(Y), 0.0});
      num_vertices++;
      texturas.push_back({0.0, 0.0}); // TODO: ??

      for (i = 0; i + nvp < num_vertices - 1; i += nvp)
        tabla_caras.push_back({i, num_vertices - 1, i + nvp});

      if (cerrar_malla) // Última cara (i = (nper - 1) * nvp)
        tabla_caras.push_back({i, num_vertices - 1, 0});
    }

    // Cara superior
    auto ult_ver = tabla_vertices[num_vertices - 2];
    if (ult_ver(X) != 0) {
      tabla_vertices.push_back({0.0, ult_ver(Y), 0.0});
      num_vertices++;
      texturas.push_back({0.0, 0.0}); // TODO: ??

      for (i = nvp - 1; i + nvp < num_vertices; i += nvp)
        tabla_caras.push_back({i, i + nvp, num_vertices - 1});

      if (cerrar_malla) // Última cara (i = nper * nvp - 1)
        tabla_caras.push_back({i, nvp - 1, num_vertices - 1});
    }
  }
}

// *****************************************************************************

Cilindro::Cilindro(const unsigned num_verts_per,
                   const unsigned nperfiles,
                   float radio_base,
                   float altura,
                   const bool crear_tapas,
                   const bool cerrar_malla,
                   const bool crear_texturas)
  : MallaRevol("malla por revolución de un cilindro")
{
  vector<Tupla3f> perfil_original(num_verts_per);
  r = radio_base;
  h = altura;

  // Construir perfil
  for (unsigned i = 0; i < num_verts_per; i++)
    perfil_original[i] = {r, h * ((float) i / (num_verts_per - 1)), 0.0};

  inicializarMallaRevol(perfil_original, nperfiles, num_verts_per,
                        crear_tapas, cerrar_malla, crear_texturas);
}

// *****************************************************************************

Cono::Cono(const unsigned num_verts_per,
           const unsigned nperfiles,
           float radio_base,
           float altura,
           const bool crear_tapas,
           const bool cerrar_malla,
           const bool crear_texturas)
  : MallaRevol("malla por revolución de un cono")
{
  vector<Tupla3f> perfil_original(num_verts_per);
  r = radio_base;
  h = altura;

  // Construir perfil
  for (unsigned i = 0; i < num_verts_per; i++) {
    // Para poner tapa arriba, dividimos por num_verts_per.
    float x = (1 - (float) i / (num_verts_per - 1)) * r;
    perfil_original[i] = {x, (- h / r) * x + h, 0.0};
  }

  inicializarMallaRevol(perfil_original, nperfiles, num_verts_per,
                        crear_tapas, cerrar_malla, crear_texturas);
}

// *****************************************************************************

Esfera::Esfera(const unsigned num_verts_per,
               const unsigned nperfiles,
               float radio,
               const bool crear_tapas,
               const bool cerrar_malla,
               const bool crear_texturas)
  : MallaRevol("malla por revolución de una esfera")
{
  vector<Tupla3f> perfil_original(num_verts_per);
  r = radio;

  // Construir perfil
  for (unsigned i = 0; i < num_verts_per; i++) {
    float y = r * (-1 + (float) 2 * i / (num_verts_per - 1));
    perfil_original[i] = {sqrt(r * r - y * y), y, 0.0};
  }

  inicializarMallaRevol(perfil_original, nperfiles, num_verts_per,
                        crear_tapas, cerrar_malla, crear_texturas);
}
