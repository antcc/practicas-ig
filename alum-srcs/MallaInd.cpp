// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Implementación de la clase MallaInd
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

// Decide si usar glBegin/glVertex/glEnd (0) o glDrawElements (1), ambos en modo inmediato
#define MODO_INMEDIATO_DRAW_ELEMENTS 1

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd( const std::string & nombreIni)
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni);
   vbo_creado = false;
   id_vbo_caras = id_vbo_vertices = 0;
}

MallaInd::MallaInd()
  : MallaInd("malla indexada, anónima") { }

// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales()
{
   // COMPLETAR: en la práctica 2: calculo de las normales de la malla
   // .......
}

void MallaInd::setColorVertices() {
  for (int i = 0; i < tabla_vertices.size(); i++) {
    color_vertices.push_back({0.0, 0.0, (float) i / (tabla_vertices.size() - 1)});
  }
}

GLuint MallaInd::VBO_Crear( GLuint tipo, GLuint tamanio, GLvoid * puntero )
{
  assert( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER );
  GLuint id_vbo ;
  glGenBuffers( 1, & id_vbo );
  glBindBuffer( tipo, id_vbo );
  glBufferData( tipo, tamanio, puntero, GL_STATIC_DRAW );
  glBindBuffer( tipo, 0 );
  return id_vbo ;
}

void MallaInd::initVBOs() {
  tam_vertices = sizeof(float) * 3L * tabla_vertices.size();
  tam_caras = sizeof(unsigned) * 3L * tabla_caras.size();

  id_vbo_vertices = VBO_Crear(GL_ARRAY_BUFFER, tam_vertices, tabla_vertices.data());
  id_vbo_caras = VBO_Crear(GL_ELEMENT_ARRAY_BUFFER, tam_caras, tabla_caras.data());

  // Atributos
  if (color_vertices.size() > 0)
    id_vbo_color_ver = VBO_Crear(GL_ARRAY_BUFFER, tam_vertices, color_vertices.data());

  vbo_creado = true;
}


// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
#if MODO_INMEDIATO_DRAW_ELEMENTS == 0

  glBegin(GL_TRIANGLES);

  glColor3fv(Tupla3f{0.0, 0.0, 0.8});
  for (auto tupla : tabla_caras) {
    for (int j = 0; j < 3; j++) {
      glVertex3fv(tabla_vertices[tupla(j)]);
    }
  }

  glEnd();

#else

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, tabla_vertices.data());

  // Especificar y habilitar puntero a colores

  glColorPointer( 3, GL_FLOAT, 0, color_vertices.data() );
  glEnableClientState( GL_COLOR_ARRAY );

  // Multiplicamos por 3 ya que en cada posición hay una 3-upla con 3 índices
  glDrawElements(GL_TRIANGLES, 3L * tabla_caras.size(), GL_UNSIGNED_INT, tabla_caras.data());

  /* NOTA: Si quisiéramos usar glDrawArrays (por ejemplo, para el modo puntos).
   * glDrawArrays(GL_TRIANGLES, 0, 3L * tabla_vertices.size());
   */

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

#endif

}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
  if (!vbo_creado)
    initVBOs();

  // Colores
  if (color_vertices.size() > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_color_ver);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_COLOR_ARRAY);
  }

  // Vértices
  glBindBuffer(GL_ARRAY_BUFFER, id_vbo_vertices);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Caras
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_caras);
  glDrawElements(GL_TRIANGLES, 3L * tabla_caras.size(), GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   GLenum mode;
   switch(cv.modoVis) {
     case modoSolido:
       mode = GL_FILL;
       break;
     case modoPuntos:
       mode = GL_POINT;
       break;
     case modoAlambre:  // modo por defecto
     default:
       mode = GL_LINE;
   }

   glPolygonMode(GL_FRONT_AND_BACK, mode);

   // Color
   setColorVertices();

   // Visualizar
   if (cv.usarVBOs)
     visualizarDE_VBOs(cv);
   else
     visualizarDE_MI(cv);
}
// *****************************************************************************

Cubo::Cubo()
  : Cubo(1.0) { }

Cubo::Cubo(float longitud_arista)
  : MallaInd("malla cubo")
{
  this->longitud_arista = longitud_arista;
  float l = longitud_arista / 2;

  tabla_vertices = {
    centro_oc + l * Tupla3f{-1.0, -1.0, -1.0},
    centro_oc + l * Tupla3f{-1.0, 1.0, -1.0},
    centro_oc + l * Tupla3f{1.0, 1.0, -1.0},
    centro_oc + l * Tupla3f{1.0, -1.0, -1.0},

    centro_oc + l * Tupla3f{-1.0, -1.0, 1.0},
    centro_oc + l * Tupla3f{-1.0, 1.0, 1.0},
    centro_oc + l * Tupla3f{1.0, 1.0, 1.0},
    centro_oc + l * Tupla3f{1.0, -1.0, 1.0},
  };

  tabla_caras = {
    {0, 1, 2}, {0, 3, 2},
    {3, 2, 6}, {3, 7, 6},
    {4, 5, 1}, {4, 0, 1},
    {4, 5, 6}, {4, 7, 6},
    {1, 5, 6}, {1, 2, 6},
    {0, 4, 7}, {0, 3, 7}
  };
}

// *****************************************************************************

Tetraedro::Tetraedro()
  : Tetraedro(1.0) { }

Tetraedro::Tetraedro(float longitud_arista)
  : MallaInd( "malla tetraedro")
{
  this->longitud_arista = longitud_arista;
  float l = longitud_arista / 2;

  tabla_vertices = {
    centro_oc + l * Tupla3f{1.0, 0.0, -1.0/sqrt(2)},
    centro_oc + l * Tupla3f{0.0, 1.0, 1.0/sqrt(2)},
    centro_oc + l * Tupla3f{-1.0, 0.0, -1.0/sqrt(2)},
    centro_oc + l * Tupla3f{0.0, -1.0, 1.0/sqrt(2)}
  };

  tabla_caras = {
    {3, 1, 0},
    {2, 1, 3},
    {0, 1, 2},
    {2, 0, 3}
  };
}
// *****************************************************************************
