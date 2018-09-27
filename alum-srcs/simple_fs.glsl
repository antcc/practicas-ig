void main()
{
   // El objetivo es escribir la variable:
   //   - \verb|gl_FragColor|: contiene el color a asignar al pixel.
   //
   // Se pueden leer (entre otras) la variable:
   //   - \verb|gl_Color| : color de la primitiva, obtenido de los colores de los vértices que la forman.

   gl_FragColor = gl_Color;
}
