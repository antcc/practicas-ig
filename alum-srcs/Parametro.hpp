#include <string>
#include <functional>
#include <matrizg.hpp>

#ifndef GRADO_LIBERTAD_HPP
#define GRADO_LIBERTAD_HPP

class Objeto3D ; //TODO: ¿es necesario?

// tipo usado para las funciones que calculan una matriz a partir de un flotante
typedef std::function< Matriz4f( float )> TFuncionCMF  ;

// -----------------------------------------------------------------------------

class Parametro
{
   private:
    static constexpr float a = 1;
    static constexpr float v0 = 1;
    static constexpr float incremento = 1;

    const std::string descripcion ;
    const bool acotado ;
    Matriz4f * const ptr_mat ; // puntero a la matriz dentro del modelo
    const TFuncionCMF fun_calculo_matriz; // produce una nueva matriz a partir de un flotante
    const float
      c, // valor inicial (y también central para grados acotados)
      s, // semiamplitud (si acotado), o factor de escala (si no acotado)
      f; // si acotado: frecuencia (ciclos por unidad del valor normalizado)
    float
      valor_norm, // p: valor actual, normalizado, no acotado (crece desde 0)
      velocidad ; // velocidad actual (se suma al valor_norm)

    void recalcular_matriz(); // recalcular M = F(q) y actualizarla en el modelo

   public:
   // crear grado de libertad no acotado
   Parametro(const std::string & p_descripcion,
             Matriz4f * p_ptr_mat,
             TFuncionCMF p_fun_calculo_matriz,
             bool p_acotado,
             float p_c,
             float p_s,
             float p_f);

   void  siguiente_cuadro();   // actualizar valor y matriz al siguiente frame
   void  reset();        // vuelve al estado inicial
   void  incrementar();  // incrementar el valor
   void  decrementar() ; // decrementar el valor
   void  acelerar();     // acelerar (aumentar velocidad normalizada)
   void  decelerar();    // decelerar (disminuir la velocidad normalizada)
   float leer_valor_actual(); // devuelve el valor actual q (escalado, no normalizado)
   float leer_velocidad_actual();    // devuelve velocidad o frecuencia actual
   std::string leer_descripcion();
};

#endif
