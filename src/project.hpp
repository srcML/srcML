/*
  project.hpp

  Copyright (C) 2005-2010  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Common definitions for the entire project, mainly for different build environments
*/

#ifndef INCLUDED_PROJECT_HPP
#define INCLUDED_PROJECT_HPP

#include <cstdlib>
#include <cctype>
#include <csignal>
#include <cerrno>

namespace pstd {

  // macros that are in different namespaces between compilers
#ifdef __GNUC__
  using std::strtol;
  using std::strtoul;
  using std::atoi;
  using std::size_t;
  using std::signal;
  using std::isspace;
#else
  using ::strtol;
  using ::strtoul;
  using ::atoi;
  using ::size_t;
  using ::signal;
  using ::isspace;
#endif

}

#endif
