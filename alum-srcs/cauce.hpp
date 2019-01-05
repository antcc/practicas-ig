// *********************************************************************
// **
// ** Gestión del cauce gráfico (clase ShaderProg) (declaraciones)
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


#ifndef CAUCE_HPP
#define CAUCE_HPP

#include <vector>
#include "aux.hpp"
#include "tuplasg.hpp"


class ShaderProg
{
   public:
   ShaderProg( const std::string & frag_fn, const std::string & vert_fn );
   void activar();

   private:
   bool  compilado ;
   GLint idProg ;
} ;


class SimpleSP : public  ShaderProg
{
   public:
   SimpleSP() ;
} ;


#endif
