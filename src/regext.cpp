#include <stdio.h>
#include <regex.h>
#include <sys/types.h>

int main(int argc, char ** argv)
{
  const char * string = argv[1];

  const char * regex = ".*\\.([:alnum:]*)";
  regex_t preg;
  int errorcode = regcomp(&preg, regex, REG_EXTENDED);
  if(errorcode == 0)
    fprintf(stderr, "Compiled\n");

  regmatch_t pmatch[2];
  errorcode = regexec(&preg, string, 2, pmatch, 0);
  if(errorcode == 0){
    fprintf(stderr, "Ran\n");
    fprintf(stderr, "Match: %d\n", pmatch[0].rm_so);
    fprintf(stderr, "Match: %d\n", pmatch[1].rm_so);
  }
  else{
    fprintf(stderr, "No Match\n");
  }
    regfree(&preg);

  return 0;
}
