// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd
{
   protected:
      unsigned nper; // numero de perfiles
      unsigned nvp; // numero de vertices por perfil

      void inicializarMallaRevol(const std::vector<Tupla3f>& perfil_original,
                                 unsigned nperfiles,
                                 unsigned num_verts_per,
                                 bool crear_tapas,
                                 bool cerrar_malla,
                                 bool crear_texturas);

      void crearMallaRevol(const std::vector<Tupla3f> & perfil_original,
                           bool crear_tapas,
                           bool cerrar_malla,
                           bool crear_texturas);

   public:
      MallaRevol(const std::string& nombre);

      MallaRevol(const std::string & nombre_arch,
                 const unsigned nperfiles,
                 const bool crear_tapas,
                 const bool cerrar_malla,
                 const bool crear_texturas);
};


// ---------------------------------------------------------------------
// clases mallas indexadas por revolución de un perfil generado proceduralmente

class Cilindro : public MallaRevol
{
  protected:
    float r; // Radio base
    float h; // Altura

  public:
    // la base tiene el centro en el origen, el radio y la altura son 1
    Cilindro(const unsigned num_verts_per,
             const unsigned nperfiles,
             float radio_base,
             float altura,
             const bool crear_tapas,
             const bool cerrar_malla,
             const bool crear_texturas);
};

class Cono : public MallaRevol
{
  protected:
    float r; // Radio base
    float h; // Altura

  public:
    // La base tiene el centro en el origen, el radio y altura son 1
    Cono(const unsigned num_verts_per,
         const unsigned nperfiles,
         float radio_base,
         float altura,
         const bool crear_tapas,
         const bool cerrar_malla,
         const bool crear_texturas);
};

class Esfera : public MallaRevol
{
  protected:
    float r; // Radio

  public:
    // La esfera tiene el centro en el origen, el radio es la unidad
    Esfera(const unsigned num_verts_per,
           const unsigned nperfiles,
           float radio,
           const bool crear_tapas,
           const bool cerrar_malla,
           const bool crear_texturas);
};

#endif
