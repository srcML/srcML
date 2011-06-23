/*
  makeargv.h

  Include file for makeargv function

  Michael L. Collard
  collard@uakron.edu
*/

#ifndef INCLUDED_MAKEARGV_H
#define INCLUDED_MAKEARGV_H

/* 
   Builds an array of strings just like argv based on s 
   Client has to deallocate array of strings returned
   memory for strings is from parameter s
*/
int makeargv(const char* s, const char* delimiter, char*** pargv);

/*
  frees up the memory allocated in makeargv
*/
void freemakeargv(char** argv);

#endif
