/*
  makeargv.cpp

  Definition of makeargv function

  Michael L. Collard
  collard@uakron.edu
*/

#include "makeargv.hpp"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int makeargv(const char* s, const char* delimiter, char*** pnargv) {

  char* t;
  int numtokens;
  int i;

  /* check for valid parameters */
  if ((s == 0) || (delimiter == 0) || (pnargv == 0)) {
    errno = EINVAL;
    return -1;
  }

  s = s + strspn(s, delimiter);
  /* copy the input string */
  if ((t = (char *)malloc(strlen(s) + 1)) == NULL)
    return -1;
  strcpy(t, s);

  /* find the number of tokens in the string */
  if (strtok(t, delimiter) != NULL)
    for (numtokens = 1; strtok(NULL, delimiter) != NULL; ++numtokens)
      ;

  /* create an argument array of strings */
  if (((*pnargv) = (char **)malloc((numtokens + 1) * sizeof(char *))) == NULL) {

    int error = errno;
    free(t);
    errno = error;

    return -1;
  }

  /* refresh t */
  strcpy(t, s);

  /* fill up the argument array with pointers to strings in t */
  if (((*pnargv)[0] = strtok(t, delimiter)) != NULL)
    for (i = 1; i < numtokens; ++i)
      (*pnargv)[i] = strtok(NULL, delimiter);
  (*pnargv)[numtokens] = NULL;

  return numtokens;
}

// free previous makeargv allocation
void freemakeargv(char** argv) {

  if (argv == NULL)
    return;
  if(argv[0] != NULL)
  {
    free(argv[0]);
  }
  free(argv);
}
