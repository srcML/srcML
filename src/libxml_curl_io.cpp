#include "libxml_curl_io.h"
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>
#include <curl/curl.h>

static CURL* curl;

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static void *myrealloc(void *ptr, size_t size);
 
static void *myrealloc(void *ptr, size_t size)
{
  /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */ 
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
}
 
static size_t
WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
  fprintf(stderr, "WRITING %d\n", nmemb);
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;
 
  mem->memory = (char*) myrealloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory) {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  }
  return realsize;
}

struct MemoryStruct chunk;
 
// check if curl matches the protocol on the URI
int curlMatch(const char * URI) {

  return (URI != NULL) && (!strncmp(URI, "https:", 6) || !strncmp(URI, "sftp:", 5));
}

// setup curl for this URI
void* curlOpen(const char * URI) {

  if (!curlMatch(URI))
    return NULL;

  chunk.memory=NULL; /* we expect realloc(NULL, size) to work */ 
  chunk.size = 0;    /* no data at this point */ 

  curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, URI);

  /* send all data to this function  */ 
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  /*
   * If you want to connect to a site who isn't using a certificate that is
   * signed by one of the certs in the CA bundle you have, you can skip the
   * verification of the server's certificate. This makes the connection
   * A LOT LESS SECURE.
   *
   * If you have a CA cert for the server stored someplace else than in the
   * default bundle, then the CURLOPT_CAPATH option might come handy for
   * you.
   */ 
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
 
  /*
   * If the site you're connecting to uses a different host name that what
   * they have mentioned in their server certificate's commonName (or
   * subjectAltName) fields, libcurl will refuse to connect. You can skip
   * this check, but this will make the connection less secure.
   */ 
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  curl_easy_perform(curl);

  return (void *) "foo";
}

// close the open file
int curlClose(void * context) {

    if (context == NULL)
      return -1;

    curl_easy_cleanup(curl);

    return 0;
}

// read from the URI
int curlRead(void * context, char * buffer, int len) {

  fprintf(stderr, "ASKINGFOR %d\n", len);

   const char *ptr = (const char *) context;

   if ((context == NULL) || (buffer == NULL) || (len < 0))
       return -1;

   if (len > chunk.size)
     len = chunk.size;
   memcpy(buffer, chunk.memory, len);
   chunk.size = 0;

   return len;
}
