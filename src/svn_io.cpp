/*
  svn_io.cpp

  Copyright (C) 2013  SDML (www.sdml.info)

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

  IO function for reading from subversion.
*/

#include <iostream>
#include <string>

#include <dlfcn.h>

#include "svn_io.hpp"
#include "Language.hpp"
#include "srcmlapps.hpp"

svn_ra_session_t * global_session;
svn_revnum_t global_revision;

struct svn_context {

  svn_stream_t * stream;
  apr_pool_t * pool;

};

int abortfunc(int retcode) {

  std::cout << retcode << '\n';

  return retcode;
}

#define DFDECLARE(F) F ## _function F ## _dynamic;

#define DFLOAD(F)  F ## _dynamic = (F ##_function)dlsym(handle, #F); \
  char* error; \
  if ((error = dlerror()) != NULL) { \
    dlclose(handle); \
    return 0; \
  }

typedef svn_error_t * (*svn_ra_get_dir2_function) (svn_ra_session_t *session, apr_hash_t **dirents,
                                                   svn_revnum_t *fetched_rev, apr_hash_t **props,
                                                   const char *path, svn_revnum_t revision,
                                                   apr_uint32_t dirent_fields, apr_pool_t *pool);
typedef svn_error_t (*svn_ra_initialize_function) (apr_pool_t *pool);
typedef svn_error_t * (*svn_config_get_config_function) (apr_hash_t **cfg_hash, const char *config_dir, apr_pool_t *pool);
typedef svn_error_t * (*svn_client_create_context_function) (svn_client_ctx_t **ctx, apr_pool_t *pool);
typedef svn_error_t * (*svn_cmdline_create_auth_baton_function) (svn_auth_baton_t **ab, svn_boolean_t non_interactive,
                              const char *username, const char *password, const char *config_dir,
                              svn_boolean_t no_auth_cache, svn_boolean_t trust_server_cert,
                              svn_config_t *cfg, svn_cancel_func_t cancel_func,
                              void *cancel_baton, apr_pool_t *pool);
typedef svn_error_t * (*svn_client_open_ra_session_function) (svn_ra_session_t **session,
                           const char *url, svn_client_ctx_t *ctx, apr_pool_t *pool);
typedef svn_error_t * (*svn_ra_stat_function) (svn_ra_session_t *session, const char *path, svn_revnum_t revision,
            svn_dirent_t **dirent, apr_pool_t *pool);
typedef svn_stringbuf_t (*svn_stringbuf_create_ensure_function) (apr_size_t minimum_size, apr_pool_t *pool);
typedef svn_stream_t * (*svn_stream_from_stringbuf_function) (svn_stringbuf_t *str, apr_pool_t *pool);
typedef svn_error_t * (*svn_ra_get_file_function) (svn_ra_session_t *session, const char *path, svn_revnum_t revision,
              svn_stream_t *stream, svn_revnum_t *fetched_rev, apr_hash_t **props, apr_pool_t *pool);
typedef svn_error_t * (*svn_stream_read_function) (svn_stream_t *stream, char *buffer, apr_size_t *len);

DFDECLARE(svn_ra_get_dir2)
DFDECLARE(svn_ra_initialize);

DFDECLARE(svn_config_get_config)
DFDECLARE(svn_client_create_context)
DFDECLARE(svn_cmdline_create_auth_baton)
DFDECLARE(svn_client_open_ra_session)
DFDECLARE(svn_ra_stat)
DFDECLARE(svn_stringbuf_create_ensure)
DFDECLARE(svn_stream_from_stringbuf)
DFDECLARE(svn_ra_get_file)
DFDECLARE(svn_stream_read)

int subversion_init() {
#if defined(__GNUG__) && !defined(__MINGW32__)
  void* handle = dlopen("libsvn_ra-1.so", RTLD_LAZY);
  if (!handle) {
    handle = dlopen("libsvn_ra-1.dylib", RTLD_LAZY);
    if (!handle) {
      fprintf(stderr, "Unable to open libxslt library\n");
      return 0;
    }
  }

  svn_ra_get_dir2_dynamic = (svn_ra_get_dir2_function)dlsym(handle, "svn_ra_get_dir2");
  char* error;
  if ((error = dlerror()) != NULL) {
    dlclose(handle);
    return 0;
  }

#else
  return 0;

#endif


}

void svn_process_dir(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool, srcMLTranslator & translator, OPTION_TYPE & options, const char * dir, const char * filename, const char * version, int language, int tabsize, int & count, int & skipped, int & error, bool & showinput, bool shownumber) {

  apr_hash_t * dirents;
  svn_revnum_t fetched_rev;
  apr_hash_t *props;

  svn_ra_get_dir2_dynamic(session, &dirents, &fetched_rev, &props, path, revision, SVN_DIRENT_ALL, pool);

  apr_hash_index_t * item;
  const void * key;
  void * value;

  for (item = apr_hash_first(pool, dirents); item; item = apr_hash_next(item)) {

    apr_hash_this(item, &key, NULL, &value);

    svn_dirent_t * dirent = (svn_dirent_t *)value;
    //svn_ra_stat(session, path, revision, &dirent, pool);

    std::string new_path = path;
    const char * name = (const char *)key;
    if(path && path[0] != 0)
      new_path += "/";
    new_path += name;

    apr_allocator_t * allocator;
    apr_allocator_create(&allocator);

    apr_pool_t * new_pool;
    apr_pool_create_ex(&new_pool, NULL, abortfunc, allocator);

    if(dirent->kind == svn_node_file)
      svn_process_file(session, new_path.c_str(), revision, new_pool, translator, options, dir, filename, version, language, tabsize, count, skipped, error, showinput, shownumber);
    else if(dirent->kind == svn_node_dir)
      svn_process_dir(session, new_path.c_str(), revision, new_pool, translator, options, dir, filename, version, language, tabsize, count, skipped, error, showinput, shownumber);
    else if(dirent->kind == svn_node_none)
      fprintf(stderr, "%s\n", "Path does not exist");
    else if(dirent->kind == svn_node_unknown)
      fprintf(stderr, "%s\n", "Unknown");

    apr_pool_destroy(new_pool);

  }

}

void svn_process_file(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool, srcMLTranslator & translator, OPTION_TYPE & options, const char * dir, const char * filename, const char * version, int language, int tabsize, int & count, int & skipped, int & error, bool & showinput, bool shownumber) {

  global_revision = revision;

  OPTION_TYPE save_options = options;
  try {

    bool foundfilename = true;

    std::string unit_filename = path;
    if (filename)
      unit_filename = filename;
    else
      unit_filename = path;

    // language based on extension, if not specified

    // 1) language may have been explicitly specified
    int reallanguage = language;

    // 2) try from the filename (basically the extension)
    if (!reallanguage)
      reallanguage = Language::getLanguageFromFilename(unit_filename.c_str());

    // error if can't find a language
    if (!reallanguage) {

      if (!isoption(options, OPTION_QUIET)) {
        if (unit_filename == "-")
          fprintf(stderr, "Skipped:  Must specify language for standard input.\n" );
        else
          fprintf(stderr, !shownumber ? "Skipped '%s':  Unregistered extension\n" :
                  "    - %s\tSkipped: Unregistered extension\n",
                  unit_filename.c_str() ? unit_filename.c_str() : "standard input");
      }

      ++skipped;

      return;
    }

    // turnon cpp namespace for non Java-based languages
    if (!(reallanguage == Language::LANGUAGE_JAVA || reallanguage == Language::LANGUAGE_ASPECTJ))
      options |= OPTION_CPP;

    // open up the file
    //void * context =
    translator.setInput(path);

    // another file
    ++count;


    const char* c_filename = clean_filename(unit_filename.c_str());

    // output the currently processed filename
    if (!isoption(options, OPTION_QUIET) && shownumber)
      fprintf(stderr, "%5d %s\n", count, c_filename);

    // translate the file
    translator.translate(path, dir,
                         foundfilename ? c_filename : 0,
                         version, reallanguage);

  } catch (FileError) {

    // output tracing information about the input file
    if (showinput && !isoption(options, OPTION_QUIET)) {

      // output the currently processed filename
      fprintf(stderr, "Path: %s", strcmp(path, STDIN) == 0 ? "standard input" : path);
      fprintf(stderr, "\tError: Unable to open file.\n");

    } else {

      if (dir)
        fprintf(stderr, "%s: Unable to open file %s/%s\n", "src2srcml", dir, path);
      else
        fprintf(stderr, "%s: Unable to open file %s\n", "src2srcml", path);
    }

    ++error;
  }

  options = save_options;

}

void svn_process_session(svn_revnum_t revision, srcMLTranslator & translator, const char * url, OPTION_TYPE & options, const char * dir, const char * filename, const char * version, int language, int tabsize, int & count, int & skipped, int & error, bool & showinput, bool shownumber) {

  apr_initialize();

  apr_allocator_t * allocator;
  apr_allocator_create(&allocator);

  apr_pool_t * pool;
  apr_pool_create_ex(&pool, NULL, abortfunc, allocator);

  svn_client_ctx_t * ctx;
  apr_hash_t * cfg_hash;
  svn_config_t * cfg_config;

  svn_ra_initialize(pool);
  svn_config_get_config(&cfg_hash, NULL, pool);
  svn_client_create_context(&ctx, pool);
  //svn_client_create_context2(&ctx, cfg_hash, pool);
  ctx->config = cfg_hash;
  cfg_config = (svn_config_t *)apr_hash_get(ctx->config, SVN_CONFIG_CATEGORY_CONFIG, APR_HASH_KEY_STRING);

  svn_boolean_t non_interactive = false;
  const char * auth_username = "";
  const char * auth_password = "";
  const char * config_dir = 0;

  svn_boolean_t no_auth_cache = false;
  svn_boolean_t trust_server_cert = true;

  svn_auth_baton_t * ab;
  svn_cmdline_create_auth_baton(&ab, non_interactive, auth_username, auth_password, config_dir, no_auth_cache, trust_server_cert,
                                cfg_config, ctx->cancel_func, ctx->cancel_baton, pool);

  ctx->auth_baton = ab;
  ctx->conflict_func = NULL;
  ctx->conflict_baton = NULL;

  svn_ra_session_t * session;
  svn_error_t * svn_error = svn_client_open_ra_session(&session, url, ctx, pool);
  global_session = session;

  if(svn_error)
    fprintf(stderr, "%s\n", svn_error->message);

  const char * path = "";
  apr_pool_t * path_pool;
  apr_pool_create_ex(&path_pool, NULL, abortfunc, allocator);

  svn_dirent_t * dirent;
  svn_ra_stat(session, path, revision, &dirent, path_pool);

  if(dirent->kind == svn_node_file)
    svn_process_file(session, path, revision, path_pool, translator, options, dir, filename, version,
                     language ? language : Language::getLanguageFromFilename(url), tabsize, count, skipped, error, showinput, shownumber);
  else if(dirent->kind == svn_node_dir)
    svn_process_dir(session, path, revision, path_pool, translator, options, dir, filename, version, language, tabsize,
                    count, skipped, error, showinput, shownumber);
  else if(dirent->kind == svn_node_none)
    fprintf(stderr, "%s\n", "Path does not exist");
  else if(dirent->kind == svn_node_unknown)
    fprintf(stderr, "%s\n", "Unknown");

  apr_terminate();

}

// check svn match
int svnReadMatch(const char * URI) {

  return 1;
}

void * svnReadOpen(const char * URI) {

  svn_context * context = new svn_context;

  apr_allocator_t * allocator;
  apr_allocator_create(&allocator);

  apr_pool_t * pool;
  apr_pool_create_ex(&pool, NULL, abortfunc, allocator);

  context->pool = pool;

  svn_stringbuf_t * str = svn_stringbuf_create_ensure(0, context->pool);
  context->stream = svn_stream_from_stringbuf(str, context->pool);

  svn_revnum_t fetched_rev;
  apr_hash_t * props;

  svn_ra_get_file(global_session, URI, global_revision, context->stream, &fetched_rev, &props, context->pool);

  return context;

}

// read from the URI
int svnRead(void * context, char * buffer, int len) {

  svn_context * ctx = (svn_context *)context;

  apr_size_t length = len;

  svn_error_t * error = svn_stream_read(ctx->stream, buffer, &length);

  if(error) return 0;

  if(length < 0) return 0;

  return length;
}

// close the open file
int svnReadClose(void * context) {

  svn_context * ctx = (svn_context *)context;

  apr_pool_destroy(ctx->pool);

  delete ctx;

  return 1;
}
