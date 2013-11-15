/*
  svn_io.cpp

  Copyright (C) 2013  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  IO function for reading from subversion.
*/

#include <apr.h>
#include <svn_fs.h>
#include <svn_repos.h>
#include <svn_ra.h>
#include <svn_client.h>
#include <svn_types.h>
#include <svn_dirent_uri.h>
#include <svn_auth.h>
#include <svn_cmdline.h>
#include <svn_string.h>
#include <svn_subst.h>

#include "srcMLTranslator.hpp"
#include "Options.hpp"

#ifndef INCLUDDED_SVN_IO_HPP
#define INCLUDDED_SVN_IO_HPP

void svn_process_dir(svn_ra_session_t * session, const char * url, const char * path, svn_revnum_t revision, apr_pool_t * pool, srcMLTranslator & translator, OPTION_TYPE & options, const char * dir, const char * filename, const char * version, int language, int tabsize, int & count, int & skipped, int & error);

void svn_process_file(svn_ra_session_t * session, const char * url, const char * path, svn_revnum_t revision, apr_pool_t * pool, srcMLTranslator & translator, OPTION_TYPE & options, const char * dir, const char * filename, const char * version, int language, int tabsize, int & count, int & skipped, int & error);

void svn_process_session(svn_revnum_t revision, srcMLTranslator & translator, const char * srcml_filename, OPTION_TYPE & options, const char * dir, const char * filename, const char * version, int language, int tabsize, int & count, int & skipped, int & error);

// check svn match
int svnReadMatch(const char * URI);

// svn open
void * svnReadOpen(const char * URI);

// read from the URI
int svnRead(void * context, char * buffer, int len);

// close the open file
int svnReadClose(void * context);

#endif
