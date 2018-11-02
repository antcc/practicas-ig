// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"
#include "grafo-escena.hpp"
#include "MallaRevol.hpp"

using namespace std ;

#define TEST_COLOREADO 1

const Tupla3f DEFAULT_COLOR = {0.02, 0.52, 0.51};

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  for (unsigned i = 0; i < entradas.size(); i++) {
    if (entradas[i].tipo == TipoEntNGE::objeto) {
      entradas[i].objeto->visualizarGL(cv);
    }
    else if (entradas[i].tipo == TipoEntNGE::transformacion) {
      glMatrixMode(GL_MODELVIEW);
      glMultMatrixf(*(entradas[i].matriz));
    }
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{
}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color )
{
  /* NOTA: Para colorear cada vértice de un color hay que llamar a la función
     setColorVertices desde el constructor de la figura que se desee, en vez de
     usar esta. */

  if (nuevo_color != color) {
    color = nuevo_color;
    for (auto entrada : entradas) {
      if (entrada.tipo == TipoEntNGE::objeto)
        entrada.objeto->fijarColorNodo(color);
    }
  }
}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
  entradas.push_back(entrada);
  return entradas.size() - 1;
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
  if (indice >= 0 && indice < entradas.size())
    if (entradas[indice].tipo == TipoEntNGE::transformacion && entradas[indice].matriz != nullptr)
      return entradas[indice].matriz;

  cout << "ERROR: práctica 3: no hay un puntero válido a una transformación en la entrada "
       << indice << endl << "Abortando..." << endl;
  exit(EXIT_FAILURE);
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto(const int ident_busc,
                                   const Matriz4f & mmodelado,
                                   Objeto3D ** objeto,
                                   Tupla3f & centro_wc)
{
   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // ........
}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************

// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
unsigned NodoGrafoEscenaParam::numParametros()
{
  return parametros.size();
}

// -----------------------------------------------------------------------------
// devuelve un puntero al i-ésimo grado de libertad
Parametro * NodoGrafoEscenaParam::leerPtrParametro( unsigned i )
{
  if (i >= 0 && i < parametros.size())
    return &parametros[i];
  return nullptr;
}

// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro()
{
  for (auto &param : parametros)
    param.siguiente_cuadro();
}

MallaTendedor::Tira::Tira() {
  agregar(MAT_Rotacion(90, 0, 0, 1));
  agregar(MAT_Escalado(grosor_tira, longitud_tira, grosor_tira));
  agregar(new Cilindro(50, 50, 1, 1, true, true));
}

MallaTendedor::TiraBorde::TiraBorde() {
  agregar(MAT_Escalado(1, grosor_tira_borde, grosor_tira_borde));
  agregar(new Tira);
}

MallaTendedor::TiraBordeEsquina::TiraBordeEsquina() {
  agregar(new TiraBorde);
  agregar(MAT_Traslacion(-(Tira::longitud_tira + 1), 0, 0));
  agregar(MAT_Escalado(1.5, 1.5, 1.5));
  auto esfera1 = new Esfera(50, 50, 1, false, true);
  agregar(esfera1);
  auto esfera2 = new Esfera(50, 50, 1, false, true);
  agregar(MAT_Traslacion((Tira::longitud_tira + 1) / 1.5, 0, 0));
  agregar(esfera2);

  /* EJEMPLO DE COLOREADO MANUAL */
  std::vector<Tupla3f> colores;
  for (unsigned i = 0; i < esfera1->numero_vertices(); i++)
    colores.push_back({(float) (i+1) / esfera1->numero_vertices(), 0.1, 0.1});
  esfera1->setColorVertices(&colores);
  esfera2->setColorVertices(&colores);
}

Armazon::Armazon() {
  // Los dos primeros bordes
  agregar(new MallaTendedor::TiraBorde);
  agregar(MAT_Traslacion(0, 0, (MallaTendedor::numero_tiras() + 1) * MallaTendedor::espacio_tiras()));
  agregar(new MallaTendedor::TiraBorde);

  // Tercer borde
  agregar(MAT_Traslacion(-MallaTendedor::longitud_tira(), 0, 0));
  agregar(MAT_Rotacion(-90, 0, 1, 0));
  agregar(new MallaTendedor::TiraBordeEsquina);
}

MallaTendedor::MallaTendedor() {
  // Malla principal y dos bordes
  agregar(new Armazon);
  for (unsigned i = 0; i < num_tiras; i++) {
    agregar(MAT_Traslacion(0, 0, esp_tiras));
    agregar(new Tira);
  }
}

constexpr float MallaTendedor::longitud_tira() {
  return Tira::longitud_tira;
}

constexpr float MallaTendedor::numero_tiras() {
  return num_tiras;
}

constexpr float MallaTendedor::espacio_tiras() {
  return esp_tiras;
}

AlaTendedor::AlaTendedor() {
  agregar(new MallaTendedor);
  agregar(MAT_Traslacion(- MallaTendedor::longitud_tira(), 0, 0));
  indice_ala = agregar(MAT_Rotacion(angulo_inicial, 0, 0, 1)); // matriz para rotación del ala
  agregar(new MallaTendedor);
}

Matriz4f* AlaTendedor::matriz_ala() {
  return leerPtrMatriz(indice_ala);
}

Pata::Pata() {
  agregar(MAT_Traslacion(-MallaTendedor::longitud_tira(), 0, 0));
  indice_pata = agregar(MAT_Rotacion(angulo_inicial, 0, 0, 1)); // matriz para rotación de la pata
  agregar(MAT_Escalado(2, 1, 1));
  auto armazon = new Armazon;
  agregar(armazon);

#if TEST_COLOREADO == 1
  armazon->fijarColorNodo(DEFAULT_COLOR);
#endif
}

Matriz4f* Pata::matriz_pata() {
  return leerPtrMatriz(indice_pata);
}

TendedorMitad::TendedorMitad() {
  auto ala = new AlaTendedor;
  m_ala = ala->matriz_ala();
  agregar(ala);

  auto pata = new Pata;
  m_pata = pata->matriz_pata();
  agregar(pata);
}

Matriz4f* TendedorMitad::matriz_ala() {
  return m_ala;
}

Matriz4f* TendedorMitad::matriz_pata() {
  return m_pata;
}

// *****************************************************************************
// Nodo raíz del modelo
// *****************************************************************************
Tendedor::Tendedor()
{
  /**
   * EJE X --> Rojo
   * EJE Y --> Verde
   * EJE Z --> Azul
   */

  ponerNombre("raíz del modelo jerárquico");

  // Rotar todo el tendedor
  auto p_rot1 = leerPtrMatriz(agregar(MAT_Ident()));

  // Posicionamiento inicial del tendedor
  agregar(MAT_Escalado(0.025, 0.025, 0.025));
  agregar(MAT_Traslacion(0, 0, -18));

  // --- Parte central del tendedor ---

  // Mitad izquierda
  auto izqda = new TendedorMitad;
  agregar(izqda);

  // Espejo
  agregar(MAT_Escalado(-1, 1, 1));

  // Mitad derecha
  auto dcha = new TendedorMitad;
  agregar(dcha);

  // --- Grados de libertad ---

  Parametro p1("rotación del primer ala", izqda->matriz_ala(),
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, -95, 85, 0.05);
  parametros.push_back(p1);

  Parametro p2("rotación del segundo ala", dcha->matriz_ala(),
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, -95, 85, 0.05);
  parametros.push_back(p2);

  Parametro p3("rotación de la primera pata", izqda->matriz_pata(),
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, 160, 20, 0.05);
  parametros.push_back(p3);

  Parametro p4("rotación de la segunda pata", dcha->matriz_pata(),
               [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
               true, 160, 20, 0.05);
  parametros.push_back(p4);

  Parametro p5("rotación de toda la figura", p_rot1,
               [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
               false, 0, 20, 0);
  parametros.push_back(p5);
}
