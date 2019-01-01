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
#include "MallaPLY.hpp"

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
  cv.pilaMateriales.push();
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

    else if (entradas[i].tipo == TipoEntNGE::material) {
      cv.pilaMateriales.activarMaterial(entradas[i].material);
    }
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  cv.pilaMateriales.pop();
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
  agregar(new Cilindro(50, 50, 1, 1, true, true, true));
}

MallaTendedor::TiraBorde::TiraBorde() {
  agregar(MAT_Escalado(1, grosor_tira_borde, grosor_tira_borde));
  agregar(new Tira);
}

MallaTendedor::TiraBordeEsquina::TiraBordeEsquina() {
  agregar(new TiraBorde);
  agregar(MAT_Traslacion(-(Tira::longitud_tira + 1), 0, 0));
  agregar(MAT_Escalado(1.5, 1.5, 1.5));
  auto esfera1 = new Esfera(50, 50, 1, false, true, true);
  agregar(esfera1);
  auto esfera2 = new Esfera(50, 50, 1, false, true, true);
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
  agregar(new Armazon);
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

Caja::PlanoCaja::PlanoCaja() {
  agregar(MAT_Traslacion(0, 0.5, 0));
  indice_plano = agregar(MAT_Ident()); // matriz de traslación de una cara de la caja
  agregar(MAT_Escalado(4, 4, 4));
  auto plano = new Plano;
  agregar(plano);

#if TEST_COLOREADO == 1
  plano->fijarColorNodo(DEFAULT_COLOR);
#endif
}

Matriz4f* Caja::PlanoCaja::matriz_plano() {
  return leerPtrMatriz(indice_plano);
}

Caja::Caja() {
  // Detrás
  agregar(MAT_Traslacion(0, 0, -2));
  auto p1 = new PlanoCaja;
  agregar(p1);

  // Delante
  agregar(MAT_Traslacion(0, 0, 4));
  auto p2 = new PlanoCaja;
  agregar(p2);

  // Derecha
  agregar(MAT_Traslacion(2, 0, -2));
  agregar(MAT_Rotacion(90, 0, 1, 0));
  auto p3 = new PlanoCaja;
  agregar(p3);

  // Izquierda
  agregar(MAT_Traslacion(0, 0, -4));
  auto p4 = new PlanoCaja;
  agregar(p4);

  // Debajo
  agregar(MAT_Traslacion(0, -1.5, 1.5));
  agregar(MAT_Rotacion(90, 1, 0, 0));
  auto p5 = new PlanoCaja;
  agregar(p5);

// Encima
  agregar(MAT_Traslacion(0, 0, -4));
  auto p6 = new PlanoCaja;
  agregar(p6);

  // Parámetros (el orden importa para el sentido de desplazamientos)
  m_caja.push_back(p2->matriz_plano());
  m_caja.push_back(p1->matriz_plano());
  m_caja.push_back(p3->matriz_plano());
  m_caja.push_back(p4->matriz_plano());
  m_caja.push_back(p5->matriz_plano());
  m_caja.push_back(p6->matriz_plano());
};

vector<Matriz4f*> Caja::matrices_caja() {
  return m_caja;
}

// *****************************************************************************
// Nodo raíz del modelo P3
// *****************************************************************************
Tendedor::Tendedor()
{
  /**
   * EJE X --> Rojo
   * EJE Y --> Verde
   * EJE Z --> Azul
   */

  ponerNombre("raíz del modelo jerárquico");

  // Unboxing del objeto
  auto caja = new Caja;
  agregar(caja);

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

  Parametro p5("rotación de todo el tendedor en el eje Y", p_rot1,
               [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
               false, 0, 20, 0);
  parametros.push_back(p5);

  auto m_caja = caja->matrices_caja(); // punteros a las matrices de traslación de la caja

  for (unsigned i = 0; i < 6; i++) {
    int sentido = i % 2 == 0 ? 1 : -1;

    Parametro p("traslación de la cara " + to_string(i+1) + " de la caja", m_caja[i],
                [=](float v) {return MAT_Traslacion(0, 0, v);},
                true, sentido * 15, sentido * 15, 0.025);
    parametros.push_back(p);
  }
}

// *****************************************************************************
// Clases para la escena P4
// *****************************************************************************

Lata::Lata() {
  ponerNombre("lata coke");

  agregar(MAT_Escalado(1.5, 1.5, 1.5));

  agregar(new MaterialTapasLata);
  agregar(new MallaRevol("../plys/lata-psup.ply", 30, false, false, true));
  agregar(new MaterialLata);
  agregar(new MallaRevol("../plys/lata-pcue.ply", 30, false, false, true));
  agregar(new MaterialTapasLata);
  agregar(new MallaRevol("../plys/lata-pinf.ply", 30, false, false, true));
}

Peones::Peones() {
  ponerNombre("peones");

  agregar(MAT_Escalado(0.5, 0.5, 0.5));

  agregar(new MaterialPeonNegro);
  agregar(new MallaRevol("../plys/peon.ply", 30, true, false, true));

  agregar(MAT_Traslacion(2.0, 0.0, 1.0));
  agregar(new MaterialPeonBlanco);
  agregar(new MallaRevol("../plys/peon.ply", 30, true, false, true));

  agregar(MAT_Traslacion(-4.0, 0.0, -2.0));
  agregar(new MaterialPeonMadera);
  agregar(new MallaRevol("../plys/peon.ply", 30, true, false, false));

}
