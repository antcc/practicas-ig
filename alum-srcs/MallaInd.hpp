// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Implementación de la clase MallaInd
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector
#include "Objeto3D.hpp"   // declaración de 'Objeto3D'

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

   protected:
      // Tablas de coordenadas de vértices y de caras
      std::vector<Tupla3f> tabla_vertices;
      std::vector<Tupla3i> tabla_caras;
      std::vector<Tupla3f> color_vertices;

      // Tablas de normales
      std::vector<Tupla3f> normales_caras;
      std::vector<Tupla3f> normales_vertices;

      // Atributos de la figura
      unsigned num_vertices;

      // Identificadores de vbo
      GLuint id_vbo_vertices;
      GLuint id_vbo_caras;
      GLuint id_vbo_color_ver;

      // Tamaño en bytes de las tablas
      unsigned tam_vertices;
      unsigned tam_caras;

      // Guarda si ya se han creado VBOs y normales
      bool vbo_creado;
      bool normales_creadas;

      // calculo de las normales de esta malla
      void calcular_normales();
      // visualizar con 'draw elements', en modo inmediato
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarDE_VBOs( ContextoVis & cv );
      // Inicializar VBOs
      void initVBOs();

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd();
      // crea una malla vacía con un nombre concreto:
      MallaInd(const std::string & nombreIni);
      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;
      // número de vértices de la malla
      unsigned numero_vertices() const;
      // Establecer colores de los vértices
      void setColorVertices(std::vector<Tupla3f> * colores = nullptr);
      // Fijar el mismo color para todos los vértices
      virtual void fijarColorNodo(const Tupla3f& color);
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   protected:
      float longitud_arista;

   public:
      Cubo();
      Cubo(float longitud_arista);
};

// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
   protected:
      float longitud_arista;

   public:
      Tetraedro();
      Tetraedro(float longitud_arista);
};

// ---------------------------------------------------------------------

class Plano : public MallaInd
{
   protected:
      float longitud_arista;

   public:
      Plano();
      Plano(float longitud_arista);
};

#endif
