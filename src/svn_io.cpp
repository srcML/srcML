/*
  svn_t.cpp

  Testing as subversion.

  Michael J. Decker
  mdecker6@kent.edu
*/

#include <iostream>
#include <string>

#include "svn_io.hpp"

int abortfunc(int retcode) {

  std::cout << retcode << '\n';

  return retcode;
}

char buffer[256];

void svn_process_dir(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool);
void svn_process_file(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool);

void svn_process_dir(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool) {

  apr_hash_t * dirents;
  svn_revnum_t fetched_rev;
  apr_hash_t *props;

  svn_ra_get_dir2(session, &dirents, &fetched_rev, &props, path, revision, SVN_DIRENT_ALL, pool);

  apr_hash_index_t * item;
  const void * key;
  void * value;

  for (item = apr_hash_first(pool, dirents); item; item = apr_hash_next(item)) {

    apr_hash_this(item, &key, NULL, &value);

    svn_dirent_t * dirent = (svn_dirent_t *)value;
    //svn_ra_stat(session, path, revision, &dirent, pool);

    std::string new_path = path;
    const char * name = (const char *)key;
    new_path += "/";
    new_path += name;

    if(dirent->kind == svn_node_file)
      svn_process_file(session, new_path.c_str(), revision, pool);
    else if(dirent->kind == svn_node_dir)
      svn_process_dir(session, new_path.c_str(), revision, pool);
    else if(dirent->kind == svn_node_none)
      fprintf(stderr, "HERE: %s %s %d %s\n", __FILE__, __FUNCTION__, __LINE__, "Path does not exist");
    else if(dirent->kind == svn_node_unknown)
      fprintf(stderr, "HERE: %s %s %d %s\n", __FILE__, __FUNCTION__, __LINE__, "Unkown");

      }

}

void svn_process_file(svn_ra_session_t * session, const char * path, svn_revnum_t revision, apr_pool_t * pool) {

  svn_stream_t * stream;
  svn_stringbuf_t * str = svn_stringbuf_create_ensure(0, pool);
  stream = svn_stream_from_stringbuf(str, pool);

  svn_revnum_t fetched_rev;
  apr_hash_t *props;
  svn_ra_get_file(session, path, revision, stream, &fetched_rev, &props, pool);

  apr_size_t len = 256;

  svn_error_t * error;

  do {

    error = svn_stream_read(stream, buffer, &len);

    if(error)
      fprintf(stderr, "HERE: %s %s %d %s\n", __FILE__, __FUNCTION__, __LINE__, error->message);

    std::cout << buffer;

  } while(len != 0);

}

void svn_process_session() {

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
  svn_cmdline_create_auth_baton(&ab, non_interactive,
                                auth_username,
                                auth_password,
                                config_dir,
                                no_auth_cache,
                                trust_server_cert,
                                cfg_config,
                                ctx->cancel_func,
                                ctx->cancel_baton,
                                pool);

  ctx->auth_baton = ab;
  ctx->conflict_func = NULL;
  ctx->conflict_baton = NULL;

  svn_ra_session_t * session;
  svn_error_t * error = svn_client_open_ra_session(&session, "http://calder.sdml.cs.kent.edu/svn/srcML", ctx, pool);

  if(error)
    fprintf(stderr, "HERE: %s %s %d %s\n", __FILE__, __FUNCTION__, __LINE__, error->message);


  const char * path = "trunk/srcdiff/trunk/src";
  apr_pool_t * path_pool;
  apr_pool_create_ex(&path_pool, NULL, abortfunc, allocator);

  svn_revnum_t revision = SVN_INVALID_REVNUM;
  svn_dirent_t * dirent;
  svn_ra_stat(session, path, revision, &dirent, path_pool);

  if(dirent->kind == svn_node_file)
    svn_process_file(session, path, revision, path_pool);
  else if(dirent->kind == svn_node_dir)
    svn_process_dir(session, path, revision, path_pool);
  else if(dirent->kind == svn_node_none)
    fprintf(stderr, "HERE: %s %s %d %s\n", __FILE__, __FUNCTION__, __LINE__, "Path does not exist");
  else if(dirent->kind == svn_node_unknown)
    fprintf(stderr, "HERE: %s %s %d %s\n", __FILE__, __FUNCTION__, __LINE__, "Unkown");

  apr_terminate();

}
