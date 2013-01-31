/*
  svn_io.hpp

  Functions for reading a file form svn

  Michael J. Decker
  mdecker6@kent.edu
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

void svn_process_dir(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool, srcMLTranslator & translator, OPTION_TYPE & options, const char * dir, const char * filename, const char * version, int language, int tabsize, int & count, int & skipped, int & error, bool & showinput, bool shownumber);

void svn_process_file(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool, srcMLTranslator & translator, OPTION_TYPE & options, const char * dir, const char * filename, const char * version, int language, int tabsize, int & count, int & skipped, int & error, bool & showinput, bool shownumber);

void svn_process_session(srcMLTranslator & translator, OPTION_TYPE & options, const char * dir, const char * filename, const char * version, int language, int tabsize, int & count, int & skipped, int & error, bool & showinput, bool shownumber);

// check svn match
int svnReadMatch(const char * URI);

// svn open
void * svneReadOpen(const char * URI);

// read from the URI
int svnRead(void * context, char * buffer, int len);

// close the open file
int svnReadClose(void * context);

#endif
