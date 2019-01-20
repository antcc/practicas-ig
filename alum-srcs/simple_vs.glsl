void main()
{
   // El objetivo es escribir estas variables:
   //    - \verb|gl_Position| posición del vértice en pantalla
   //    - \verb|gl_FrontColor| color asociado al vértice
   //
   // Se pueden leer (entre otras) las variables:
   //    - \verb|gl_ModelViewProjection| : matriz actual de transformación de coordenadas
   //    - \verb|gl_Vertex| : coordenadas del punto enviadas por la aplicación
   //    - \verb|gl_Color| : color actual especificado con 'glColor'

   gl_FrontColor = gl_Color ;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
