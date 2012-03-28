/*
  srcmlapps.cpp

  Copyright (C) 2004-2011  SDML (www.sdml.info)

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

  Common utilities for srcML applications.
*/

#include "srcmlapps.hpp"
#include <cstring>
#include <clocale>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <sys/stat.h>

void checkargisoption(const char* name, const char* opt, const char* optarg, int optind, int lastoptind) {

      // check for missing argument confused by an argument that looks like an option
      if (optind == (lastoptind + 2) && argisoption(optarg)) {
	fprintf(stderr, "%s: Argument '%s' to option '%s' appears to be another option.\n"
		"If this is correct, use the direct form '%s=%s\n",
		name, optarg, opt, opt, optarg);
	exit(1);
      }
}

void checkargisnonempty(const char* name, const char* opt, const char* optarg, int optind, int lastoptind) {

      // check for missing argument confused by an argument that looks like an option
  if (optind == (lastoptind + 1) && strcmp(optarg, "") == 0) {
    fprintf(stderr, "%s: Empty argument to option '%s'.\n",
	    name, opt);
    exit(1);
  }
}

const char* clean_filename(const char* in) {

  const char* pos = in;
  int len = strlen(in);

  while (len > 2 && pos[0] == '.' && pos[1] == PATH_SEPARATOR) {
    pos += 2;
    len -= 2;
  }

  while (len > 3 && pos[0] == '.' && pos[1] == '.' && pos[2] == PATH_SEPARATOR) {
    pos += 3;
    len -= 3;
  }

  return pos;
}

const char * srcml_version() {

  return REVISION;

}
