#include <stdio.h>
#include <regex.h>
#include <sys/types.h>
#include <cstring>

int main(int argc, char ** argv)
{
  int length = strlen(argv[1]);
  char * string = argv[1];
  for(int i = 0; i < length / 2; ++i)
    {
      char temp = string[i];
      string[i] = string[length - (i + 1)];
      string[length - (i + 1)] = temp;
    }
  fprintf(stderr, "%s\n", string);

  //  const char * regex = "(zg\\.|2zb\\.)*([:alnum:]*)\\.";
  const char * regex = "(zg\\.|2zb\\.)*([^\\.]*)";
  regex_t preg;
  int errorcode = regcomp(&preg, regex, REG_EXTENDED);
  if(errorcode == 0)
    fprintf(stderr, "Compiled\n");

  regmatch_t pmatch[3];
  errorcode = regexec(&preg, string, 3, pmatch, 0);
  if(errorcode == 0){
    fprintf(stderr, "Ran\n");
    fprintf(stderr, "Match: %d:%d\n", pmatch[0].rm_so, pmatch[0].rm_eo);
    fprintf(stderr, "Match: %d:%d\n", pmatch[1].rm_so, pmatch[1].rm_eo);
    fprintf(stderr, "Match: %d:%d\n", pmatch[2].rm_so, pmatch[2].rm_eo);
  }
  else{
    fprintf(stderr, "No Match\n");
  }
  
  for(int i = pmatch[2].rm_so; i < (pmatch[2].rm_eo + pmatch[2].rm_so) / 2; ++i)
  {
      char temp = string[i];
      fprintf(stderr, "%c\n", temp);
      fprintf(stderr, "%c\n", string[pmatch[2].rm_eo - ((i + 1) - pmatch[2].rm_so)]);
      string[i] = string[pmatch[2].rm_eo - ((i + 1) - pmatch[2].rm_so)];
      string[pmatch[2].rm_eo - ((i + 1) - pmatch[2].rm_so)] = temp;
  }

    string [pmatch[2].rm_eo] = '\0';

  fprintf(stderr, "%*s\n", pmatch[2].rm_eo - pmatch[2].rm_so, string + pmatch[2].rm_so);
    regfree(&preg);
  return 0;
}
