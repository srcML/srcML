/*
  srcmlapps.cpp

  Copyright (C) 2004, 2005  SDML (www.sdml.info)

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

#include "srcmlapps.h"
#include <cstring>
#include <clocale>
#include <cstdio>
#include <cctype>
#include <sys/stat.h>

#ifdef __GNUC__
#include <langinfo.h>
#endif

// current environment encoding
const char* getencoding() {

#ifdef __GNUC__
  setlocale(LC_ALL, "");

  return nl_langinfo(CODESET);
#else
  return "ISO-8859-1";
#endif
}

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

char** makeargv(char* s) {

  fprintf(stderr, "LINE:%s\n", s);
  // figure out how many elements to allocate in the resulting array
  int count = 0;
  bool instring = false;
  char prevchar = 'a';
  for (char* p = s; p; ++p) {

    // toggle back and forth between strings
    if (*p == '"')
      instring = !instring;

    if (!instring && isspace(*p) && !isspace(prevchar))
      ++count;

    prevchar = *p;
  }

  // allocate the array of strings based on the count
  char** argv = new char*[count + 1];
  argv[0] = 0;

  return argv;
  // now point into our original string
  count = 0;
  instring = false;
  prevchar = 'a';
  char* start = s;
  for (char* p = s; p; ++p) {

    // toggle back and forth between strings
    if (*p == '"')
      instring = !instring;

    if (!instring && isspace(*p) && !isspace(prevchar)) {
      argv[count++] = start;
      *p = '\0';
      start = (p + 1);
    }

    prevchar = *p;
  }
  argv[count] = 0;

  return argv;
}

// create the directories from the full path
void makedirectories(char * path)
{
  // construct the directory subpath by subpath
  for (char* c = path; *c; ++c) {

    // replace the path delimiter with a null, mkdir, then put back
    if (*c == '/') {
      *c = '\0';
      mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      *c = '/';
    }
  }
}

const char* clean_filename(const char* in) {

  const char* pos = in;
  int len = strlen(in);

  while (len > 2 && pos[0] == '.' && pos[1] == '/') {
    pos += 2;
    len -= 2;
  }

  while (len > 3 && pos[0] == '.' && pos[1] == '.' && pos[2] == '/') {
    pos += 3;
    len -= 3;
  }

  return pos;
}


