// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Implementación de la clase MallaInd
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

using namespace std;

// Decide si usar glBegin/glVertex/glEnd (0) o glDrawElements (1), ambos en modo inmediato
#define MODO_INMEDIATO_DRAW_ELEMENTS 1

// *****************************************************************************
// funciones auxiliares

GLuint VBO_Crear( GLuint tipo, GLuint tamanio, GLvoid * puntero )
{
  assert( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER );
  GLuint id_vbo ;
  glGenBuffers( 1, & id_vbo );
  glBindBuffer( tipo, id_vbo );
  glBufferData( tipo, tamanio, puntero, GL_STATIC_DRAW );
  glBindBuffer( tipo, 0 );
  return id_vbo ;
}

Tupla3f calcularCentroCajaEnglobante(const std::vector<Tupla3f>& vertices)
{
  assert(vertices.size() > 0);

  Tupla3f maximo = vertices[0],
          minimo = vertices[0]; // puntos diagonales opuestos

  for (auto ver : vertices) {
    maximo = {max(ver(X), maximo(X)), max(ver(Y), maximo(Y)), max(ver(Z), maximo(Z))};
    minimo = {min(ver(X), minimo(X)), min(ver(Y), minimo(Y)), min(ver(Z), minimo(Z))};
  }

  return (maximo + minimo) / 2.0;
}

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd(const std::string & nombreIni)
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni);
   vbo_creado = false;
   normales_creadas = false;
   usar_texturas = false;
   id_vbo_caras = id_vbo_vertices = id_vbo_color_ver =
     id_vbo_normales = id_vbo_texturas = 0;
   tam_caras = tam_vertices = 0;
   num_vertices = 0;
}

// -----------------------------------------------------------------------------

MallaInd::MallaInd()
  : MallaInd("malla indexada, anónima") { }

// -----------------------------------------------------------------------------

void MallaInd::calcular_normales()
{
  //Inicializar normales de vértices
  normales_vertices.insert(normales_vertices.begin(), num_vertices, {0.0, 0.0 , 0.0});

  // Normales de caras
  for (auto cara : tabla_caras) {
    Tupla3f
      a = tabla_vertices[cara(Y)] - tabla_vertices[cara(X)],
      b = tabla_vertices[cara(Z)] - tabla_vertices[cara(X)],
      v = a.cross(b);

    if (v.lengthSq() > 0.0)
      v = v.normalized();

    normales_caras.push_back(v);

    // Normales de vértices
    for (int i = 0; i < 3; i++)
      normales_vertices[cara(i)] = normales_vertices[cara(i)] + v;
  }

  // Normales de vértices (normalización)
  for (auto &nv : normales_vertices)
    if (nv.lengthSq() > 0.0)
      nv = nv.normalized();

  normales_creadas = true;
}

// -----------------------------------------------------------------------------

void MallaInd::initVBOs() {
  tam_vertices = sizeof(float) * 3L * num_vertices;
  tam_caras = sizeof(unsigned) * 3L * tabla_caras.size();

  id_vbo_vertices = VBO_Crear(GL_ARRAY_BUFFER, tam_vertices, tabla_vertices.data());
  id_vbo_caras = VBO_Crear(GL_ELEMENT_ARRAY_BUFFER, tam_caras, tabla_caras.data());

  // Atributos

  if (color_vertices.size() > 0)
    id_vbo_color_ver = VBO_Crear(GL_ARRAY_BUFFER, tam_vertices, color_vertices.data());

  if (normales_vertices.size() > 0)
    id_vbo_normales = VBO_Crear(GL_ARRAY_BUFFER, tam_vertices, normales_vertices.data());

  if (texturas.size() > 0)
    id_vbo_texturas =
      VBO_Crear(GL_ARRAY_BUFFER, sizeof(float) * 2L * num_vertices, texturas.data());

  vbo_creado = true;
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_Plano(ContextoVis & cv)
{
  glBegin(GL_TRIANGLES);

  for (unsigned i = 0; i < tabla_caras.size(); i++) {
    glNormal3fv(normales_caras[i]);
    for (unsigned j = 0; j < 3; j++) {
      unsigned iv = tabla_caras[i](j);
      if (texturas.size() > 0) glTexCoord2fv(texturas[iv]);
      if (color_vertices.size() > 0) glColor3fv(color_vertices[iv]);
      glVertex3fv(tabla_vertices[iv]);
    }
  }

  glEnd();
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
#if MODO_INMEDIATO_DRAW_ELEMENTS == 0

  glBegin(GL_TRIANGLES);

  for (unsigned i = 0; i < tabla_caras.size(); i++) {
    for (unsigned j = 0; j < 3; j++) {
      unsigned iv = tabla_caras[i](j);

      if (usar_texturas) {
        glNormal3fv(normales_vertices[iv]);
        glTexCoord2fv(texturas[iv]);
      }

      if (color_vertices.size() > 0 && ! cv.modoSeleccionFBO)
        glColor3fv(color_vertices[iv]);

      glVertex3fv(tabla_vertices[iv]);
    }
  }

  glEnd();

#else

  // Especificar y habilitar puntero a colores o normales

  if (usar_texturas) {
    glEnableClientState( GL_NORMAL_ARRAY );
    glNormalPointer( GL_FLOAT, 0, normales_vertices.data() );

    if (texturas.size() > 0) {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, texturas.data());
    }
  }

  if (color_vertices.size() > 0 && ! cv.modoSeleccionFBO) {
    glColorPointer(3, GL_FLOAT, 0, color_vertices.data());
    glEnableClientState( GL_COLOR_ARRAY );
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, tabla_vertices.data());
  // Multiplicamos por 3 ya que en cada posición hay una 3-upla con 3 índices
  glDrawElements(GL_TRIANGLES, 3L * tabla_caras.size(), GL_UNSIGNED_INT, tabla_caras.data());

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

#endif

}

// ----------------------------------------------------------------------------

void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
  if (!vbo_creado)
    initVBOs();

  // Colores o normales
  if (usar_texturas) {
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo_normales);
    glNormalPointer(GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);visualizarDE_MI(cv);

    if (texturas.size() > 0) {
      glBindBuffer(GL_ARRAY_BUFFER, id_vbo_texturas);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }

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
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   // Texturas
   usar_texturas = cv.modoVis == modoIluminacionPlano || cv.modoVis == modoIluminacionSuave;

   if (!usar_texturas) {
     glDisable(GL_LIGHTING);
     glDisable(GL_TEXTURE_2D);
   }
   else if (!normales_creadas)
     calcular_normales();

   // Modo selección P5
   if (cv.modoSeleccionFBO) {
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     glShadeModel(GL_FLAT);
     visualizarDE_MI(cv);
     return;
   }

   GLenum mode;
   switch(cv.modoVis) {
     case modoSolido:
       mode = GL_FILL;
       glShadeModel(GL_SMOOTH);
       break;
     case modoPuntos:
       mode = GL_POINT;
       break;
     case modoIluminacionPlano:
       mode = GL_FILL;
       glShadeModel(GL_FLAT);
       break;
     case modoIluminacionSuave:
       mode = GL_FILL;
       glShadeModel(GL_SMOOTH);
       break;
     case modoAlambre:  // modo por defecto
     default:
       mode = GL_LINE;
       glShadeModel(GL_SMOOTH);
   }

   glPolygonMode(GL_FRONT_AND_BACK, mode);

   // Visualizar
   if (cv.usarVBOs)
     visualizarDE_VBOs(cv);
   else if (cv.modoVis == modoIluminacionPlano)
     visualizarDE_MI_Plano(cv);
   else
     visualizarDE_MI(cv);
}

// -----------------------------------------------------------------------------

unsigned MallaInd::numero_vertices() const {
  return num_vertices;
}

// -----------------------------------------------------------------------------

void MallaInd::setColorVertices(std::vector<Tupla3f> * colores)
{
  color_vertices.clear();
  if (colores != nullptr) {
    for (unsigned i = 0; i < num_vertices; i++)
      color_vertices.push_back(colores->at(i));
  }

  else if (color_vertices.size() == 0) { // Gradiente
    for (unsigned i = 0; i < num_vertices; i++)
      color_vertices.push_back({0.1, 0.1, (float) (i+1) / num_vertices});
  }
}

// -----------------------------------------------------------------------------

void MallaInd::fijarColorNodo(const Tupla3f& color) {
  color_vertices.clear();
  for (unsigned i = 0; i < num_vertices; i++)
    color_vertices.push_back(color);
}

// -----------------------------------------------------------------------------

void MallaInd::calcularCentroOC() {
  if (!centro_calculado) {
    ponerCentroOC(calcularCentroCajaEnglobante(tabla_vertices));
    centro_calculado = true;
  }
}

// *****************************************************************************

Cubo::Cubo()
  : Cubo(1.0) { }

// -----------------------------------------------------------------------------

Cubo::Cubo(float longitud_arista)
  : MallaInd("malla cubo")
{
  num_vertices = 8;
  this->longitud_arista = longitud_arista;
  float l = longitud_arista / 2;

  tabla_vertices = {
    l * Tupla3f{-1.0, -1.0, -1.0},
    l * Tupla3f{-1.0, 1.0, -1.0},
    l * Tupla3f{1.0, 1.0, -1.0},
    l * Tupla3f{1.0, -1.0, -1.0},

    l * Tupla3f{-1.0, -1.0, 1.0},
    l * Tupla3f{-1.0, 1.0, 1.0},
    l * Tupla3f{1.0, 1.0, 1.0},
    l * Tupla3f{1.0, -1.0, 1.0},
  };

  tabla_caras = {
    {0, 1, 2}, {2, 3, 0},
    {3, 2, 6}, {7, 3, 6},
    {4, 5, 1}, {0, 4, 1},
    {4, 6, 5}, {4, 7, 6},
    {1, 5, 6}, {6, 2, 1},
    {4, 0, 7}, {7, 0, 3}
  };

  // Color
  setColorVertices();
}

// *****************************************************************************

Tetraedro::Tetraedro()
  : Tetraedro(1.0) { }

// -----------------------------------------------------------------------------

Tetraedro::Tetraedro(float longitud_arista)
  : MallaInd( "malla tetraedro")
{
  num_vertices = 4;
  this->longitud_arista = longitud_arista;
  float l = longitud_arista / 2;

  tabla_vertices = {
    l * Tupla3f{1.0, 0.0, -1.0/sqrt(2)},
    l * Tupla3f{0.0, 1.0, 1.0/sqrt(2)},
    l * Tupla3f{-1.0, 0.0, -1.0/sqrt(2)},
    l * Tupla3f{0.0, -1.0, 1.0/sqrt(2)}
  };

  tabla_caras = {
    {3, 0, 1},
    {2, 3, 1},
    {1, 0, 2},
    {2, 0, 3}
  };

  // Color
  setColorVertices();
}

// *****************************************************************************

Plano::Plano()
  : Plano(1.0) { }

// -----------------------------------------------------------------------------

Plano::Plano(float longitud_arista)
  : MallaInd("malla plano")
{
  num_vertices = 4;
  this->longitud_arista = longitud_arista;
  float l = longitud_arista / 2;

  tabla_vertices = {
    l * Tupla3f{-1.0, -1.0, 0.0},
    l * Tupla3f{-1.0, 1.0, 0.0},
    l * Tupla3f{1.0, 1.0, 0.0},
    l * Tupla3f{1.0, -1.0, 0.0},
  };

  tabla_caras = { {0, 2, 1}, {0, 3, 2} };

  // Color
  setColorVertices();
}
