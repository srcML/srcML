#include <stdio.h>
#include <regex.h>
#include <sys/types.h>
#include <string.h>

#define MAX_MATCHES 100

int main(int argc, char ** argv) {

  char* pattern = argv[1];
  char* data = argv[2];

  /* compile the regular expression pattern */
  regex_t regexp;
  int errorcode = regcomp(&regexp, pattern, REG_EXTENDED);
  if (errorcode == 0)
    fprintf(stderr, "Compiled\n");

  /* space for resulting offsets of matches */
  regmatch_t pmatch[MAX_MATCHES];

  /* evaluate the compiled regular expression on the data */
  errorcode = regexec(&regexp, data, MAX_MATCHES, pmatch, 0);
  if (errorcode) {
    fprintf(stderr, "No Match\n");
    regfree(&regexp);
    return 1;
  }

  /* output the matched results */
  for (int i = 0; i < MAX_MATCHES && pmatch[i].rm_so != -1; ++i) {

      fprintf(stderr, "Match Location: %d:%d\n", (int) pmatch[i].rm_so, (int) pmatch[i].rm_eo);
      fprintf(stderr, "Match: %*s\n", pmatch[i].rm_eo - pmatch[i].rm_so, data + pmatch[i].rm_so);
  }

  /* free up internal regular expression memory */
  regfree(&regexp);

  return 0;
}
