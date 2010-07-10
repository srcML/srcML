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
 
/**
 * sqlMatch:
 * @URI: an URI to test
 *
 * Check for an sql: query
 *
 * Returns 1 if yes and 0 if another Input module should be used
 */
int
curlMatch(const char * URI) {

  return (URI != NULL) && (!strncmp(URI, "http:", 5) || !strncmp(URI, "sftp:", 5) || !strncmp(URI, "https:", 6));
}

/**
 * curlOpen:
 * @URI: an URI to test
 *
 * Return a pointer to the curl: query handler, in this example simply
 * the current pointer...
 *
 * Returns an Input context or NULL in case or error
 */
void *
curlOpen(const char * URI) {
    if ((URI == NULL) || (strncmp(URI, "http:", 5)))
        return(NULL);

chunk.memory=NULL; /* we expect realloc(NULL, size) to work */ 
chunk.size = 0;    /* no data at this point */ 
 

    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, URI);

    /* send all data to this function  */ 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
    /* we pass our 'chunk' struct to the callback function */ 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    curl_easy_perform(curl);

    return (void *) "foo";
}

/**
 * curlClose:
 * @context: the read context
 *
 * Close the curl: query handler
 *
 * Returns 0 or -1 in case of error
 */
int
curlClose(void * context) {

    if (context == NULL)
      return -1;

    curl_easy_cleanup(curl);

    return 0;
}

/**
 * curlRead:
 * @context: the read context
 * @buffer: where to store data
 * @len: number of bytes to read
 *
 * Implement an curl: query read.
 *
 * Returns the number of bytes read or -1 in case of error
 */
int
curlRead(void * context, char * buffer, int len) {
   const char *ptr = (const char *) context;

   if ((context == NULL) || (buffer == NULL) || (len < 0))
       return -1;

   if (len < chunk.size)
     len = chunk.size;
   memcpy(buffer, chunk.memory, len);
   chunk.size = 0;

   return len;
}
