/*
  process_svn.hpp

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

#ifndef INCLUDDED_PROCESS_SVN_HPP
#define INCLUDDED_PROCESS_SVN_HPP

void svn_process_dir(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool);
void svn_process_file(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool);

void svn_process_session();

#endif
