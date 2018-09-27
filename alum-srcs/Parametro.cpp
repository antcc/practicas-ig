#include <cassert>
#include <cmath>
#include "Parametro.hpp"
#include "Objeto3D.hpp"

// -----------------------------------------------------------------------------

// crear grado de libertad no acotado

Parametro::Parametro(
   const std::string &  p_descripcion,
   Matriz4f *           p_ptr_mat,
   TFuncionCMF          p_fun_calculo_matriz,
   bool                 p_acotado,
   float                p_c,
   float                p_s,
   float                p_f
)

{
   // COMPLETAR: práctica 3: inicializar un parámetro
   // ....

}
// -----------------------------------------------------------------------------

void Parametro::siguiente_cuadro()
{
   // COMPLETAR: práctica 3: actualizar el valor y la matriz para el siguiente cuadro
   // ....

}
// -----------------------------------------------------------------------------

void Parametro::reset()
{
   // COMPLETAR: práctica 3: hace reset de valor y velocidad
   // ....

}
// -----------------------------------------------------------------------------
void Parametro::incrementar()
{
   // COMPLETAR: práctica 3: incrementa el valor
   // ....

}
// -----------------------------------------------------------------------------

void Parametro::decrementar()
{
   // COMPLETAR: práctica 3: decrementa el valor
   // ....

}
// -----------------------------------------------------------------------------
void Parametro::acelerar()
{
   // COMPLETAR: práctica 3: incrementa la velocidad
   // ....

}
// -----------------------------------------------------------------------------
void Parametro::decelerar()
{
   // COMPLETAR: práctica 3: decrementa la velocidad
   // ....

}
// -----------------------------------------------------------------------------

float Parametro::leer_valor_actual()
{
   // COMPLETAR: práctica 3: devuelve el valor actual
   // ....

}

// -----------------------------------------------------------------------------
float Parametro::leer_velocidad_actual()
{
   // COMPLETAR: práctica 3: devuelve la velocidad actual
   // ....

}
// -----------------------------------------------------------------------------

std::string Parametro::leer_descripcion()
{
   // COMPLETAR: práctica 3: devuelve la descripción
   // ....

}
// -----------------------------------------------------------------------------
