########################################################################
##
## archivo 'include.make'
## Carlos Ureña,
## modificado por última vez: Septiembre de 2017


## *********************************************************************
##
## invalidar definiciones de pattern rules predefinidias, definir targets

.SUFFIXES:
.PHONY: x, all, compile_all

## ********************************************************************
## carpetas

srcs_dir      := ../srcs
include_dir   := ../include
objs_dir      := ../objs
exe_dir       := ../bin

## *********************************************************************
## nombre del binario ejecutable
target_name := $(exe_dir)/prac_exe

## *********************************************************************
## nombre de las unidades de compilación (en 'srcs') que se deben enlazar
units := aux\
         jpg_imagen jpg_memsrc jpg_readwrite\
         shaders matrices-tr\
         file_ply_stl

## *********************************************************************
## unidades de compilación (.o a crear en 'objs')

objs_no_loc   := $(addprefix $(objs_dir)/, $(addsuffix .o, $(notdir $(units))))
objs_loc_nd   := $(addsuffix .o, $(notdir $(units_alu)))
objs_loc      := $(addprefix $(objs_dir)/, $(objs_loc_nd))
objs          := $(objs_loc) $(objs_no_loc)  $(objs_jpg)

## *********************************************************************
## definiciones dependientes del SO

uname:=$(shell uname -s)

lib_glfw = -lglfw

ifeq ($(uname),Darwin)
   os          := OSX
   lib_gl      := -framework OpenGL
   ##lib_glut    := -framework GLUT
   lib_glu     := /System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGLU.dylib
   lib_aux     := $(lib_glu)
   depr        := -Wdeprecated-declarations
   compat      := $(compatibilidad)
   comp        := clang++
   extra_inc_dir := -I /opt/local/include
else
    os          := LINUX
    lib_gl      := -lGL
    ##lib_glut    := -lglut
    lib_glu     := -lGLU
    lib_aux     := -lGLEW $(lib_glu)
    compat      := $(compatibilidad)
    comp        := g++
	 extra_inc_dir :=
endif

lib_jpg         := -L/opt/local/lib -ljpeg

## flags enlazador (librerías)
ld_flags  := $(lib_dir_loc) $(lib_aux) $(lib_glfw) $(lib_gl) $(lib_jpg)

## flags compilador:
os_flag  := -D$(os)
c_flags  := $(compat) -I$(include_dir) $(extra_inc_dir) $(os_flag) $(opt_dbg_flag) $(exit_first) $(warn_all)


## *********************************************************************
## auxiliares:

raya:=---------------------------------------------------------------

##
## funcion para compilar
## llamar con:
##     $(call compilar, $<, $@)

compilar =\
   echo `tput bold`$(raya);\
   echo Compilando: $(notdir $(1));\
   tput sgr0 ;\
   echo $(comp) $(c_flags) -c $(1) -o $(2) ;\
   $(comp) $(c_flags) -c $(1) -o $(2)

## *********************************************************************
## targets

x: $(target_name)
	@echo "S.O.       :" $(os)
	@echo "Ejecutando :" $(target_name) " ...."
	./$(target_name)

all:
	make clean
	make compile_all

compile_all: $(target_name)

## *********************************************************************
## enlazar

$(target_name) : $(objs)
	@echo `tput bold`$(raya)
	@echo "S.O.    : " $(os)
	@echo Enlazando: $(target_name) ....
	@tput sgr0
	$(comp) -o $(target_name) $(objs) $(ld_flags)
	@echo $(raya)

## *********************************************************************
## compilar una unidad

## localmente (fuente en el directorio de trabajo)

$(objs_dir)/%.o: %.cpp
	@$(call compilar,$<,$@)

## ---------------------------------------------------------------------
## no localmente (fuente en $(srcs_dir), obj en $(objs_dir)

$(objs_dir)/%.o: $(srcs_dir)/%.cpp
	@$(call compilar,$<,$@)

$(objs_dir)/%.o: $(srcs_dir)/%.cc
	@$(call compilar,$<,$@)

$(objs_dir)/%.o: $(srcs_dir)/%.c
	@$(call compilar,$<,$@)

## ---------------------------------------------------------------------
## en el directorio 'srcs'

$(objs_dir)/%.o: $(jpg_srcs_dir)/%.c
	@$(call compilar,$<,$@)

$(objs_dir)/%.o: $(jpg_srcs_dir)/%.cpp
	@$(call compilar,$<,$@)

## *********************************************************************
## limpiar

clean:
	rm -f $(objs_dir)/*.o *.o $(target_name)
