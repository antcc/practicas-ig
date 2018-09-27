## *********************************************************************
##
## prácticas IG GIM (18-19) - makefile para las prácticas
## (Julio de 2018)
##

## ---------------------------------------------------------------------


# nota: se debe añadir a 'units_alu' los nombres de las unidades de compilación
# que se quieran enlazar para crear el ejecutable 'prac_exe'. Por ejemplo:
# Objeto3D, MallaInd, practica2, MallaRevol, MallaPLY, practica3, etc......
# (para cada nombre habrá un archivo .cpp en 'alum-src')

units_alu := main cauce\
             practica1 Objeto3D MallaInd\
             practica2 MallaRevol MallaPLY\
             practica3 Parametro grafo-escena\
             practica4 materiales\
             practica5 Camara CamaraInter

## ---------------------------------------------------------------------
## aspectos configurables

opt_dbg_flag   := -g             # seleccionar depuración (-g) u optimización (-O3)
exit_first     := -Wfatal-errors # sirve para abortar después de primer error
warn_all       := -Wall          # sirve para ver todos los warnings
compatibilidad := -std=c++11     # versión del estandar C++ admitida

## ---------------------------------------------------------------------
## invocar las definiciones y targets en el archivo 'include.make'

include ../include/include.make
