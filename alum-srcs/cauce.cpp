// *********************************************************************
// **
// ** Gestión del cauce gráfico (clase ShaderProg) (implementación)
// **
// ** Copyright (C) 2017 Carlos Ureña
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



#include "cauce.hpp"
#include "shaders.hpp"

// -----------------------------------------------------------------------------

ShaderProg::ShaderProg( const std::string & frag_fn, const std::string & vert_fn )
{
   compilado = false ;
   idProg = CrearPrograma( frag_fn.c_str(), vert_fn.c_str()) ;
   compilado = true ;
}
// -----------------------------------------------------------------------------

void ShaderProg::activar()
{
   assert( compilado );
   using namespace std ;
   glUseProgram( idProg );
   //cout << "activado frag/vert shader" << endl ;
}
// -----------------------------------------------------------------------------

SimpleSP::SimpleSP()

: ShaderProg( "simple_fs.glsl", "simple_vs.glsl")
{
   using namespace std ;
   //cout << "creado shader program simple" << endl << flush ;
}
