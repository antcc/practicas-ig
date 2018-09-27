// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
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

      // Identificadores de vbo
      GLuint id_vbo_vertices;
      GLuint id_vbo_caras;
      GLuint id_vbo_color_ver;

      // Tamaño en bytes de las tablas
      unsigned tam_vertices;
      unsigned tam_caras;

      // Guarda si ya se han creado los vbos para modo diferido
      bool vbo_creado;

      // calculo de las normales de esta malla
      void calcular_normales();

      // Crear VBOS necesarios
      void crearVBOs();

      // visualizar con 'draw elements', en modo inmediato
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarDE_VBOs( ContextoVis & cv );
      // Crear VBOs
      GLuint VBO_Crear( GLuint tipo, GLuint tamanio, GLvoid * puntero );
      // Inicializar VBOs
      void initVBOs();
      // Establecer colores de los vértices
      void setColorVertices();

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd();
      // crea una malla vacía con un nombre concreto:
      MallaInd(const std::string & nombreIni);
      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   protected:
      static const int num_vertices = 8;
      float longitud_arista;
   public:
      Cubo();
      Cubo(float longitud_arista);
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
   protected:
      static const int num_vertices = 4;
      float longitud_arista;
   public:
      Tetraedro();
      Tetraedro(float longitud_arista);
};


#endif
