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
// funciones auxiliares
Tupla3f calcularCentroCajaEnglobante(const std::vector<Tupla3f>& vertices);

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

      // Coordenadas de texturas
      std::vector<Tupla2f> texturas;

      // Atributos de la figura
      unsigned num_vertices;
      bool usar_texturas;

      // Identificadores de vbo
      GLuint id_vbo_vertices;
      GLuint id_vbo_caras;
      GLuint id_vbo_color_ver;
      GLuint id_vbo_normales;
      GLuint id_vbo_texturas;

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
      // Visualizar con glBegin/glEnd en modo inmediato
      void visualizarDE_MI_Plano(ContextoVis & cv);
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
      // Recalcular el centro del objeto
      virtual void calcularCentroOC();
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

// EXAMEN P45

class Dado : public MallaInd
{
  public:
    Dado();
};

#endif
