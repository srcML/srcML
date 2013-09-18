/*
  test_srcMLState.cpp

  Unit tests for srcMLState.

 */

#include <srcMLState.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>

int main(int argc, char * argv[]) {

  /*
    getParen
   */ 

  {
    srcMLState s;
    assert(s.getParen() == 0);
  }

  /*
    incParen
   */ 

  {
    srcMLState s;
    s.incParen();
    assert(s.getParen() == 1);
  }

  /*
    decParen
   */ 

  {
    srcMLState s;
    s.incParen();
    s.incParen();
    s.decParen();
    assert(s.getParen() == 1);
  }

  /*
    getTypeCount
   */ 

  {
    srcMLState s;
    assert(s.getTypeCount() == 0);
  }

  /*
    incTypeCount
   */ 

  {
    srcMLState s;
    s.incTypeCount();
    assert(s.getTypeCount() == 1);
  }

  /*
    decTypeCount
   */ 

  {
    srcMLState s;
    s.incTypeCount();
    s.incTypeCount();
    s.decTypeCount();
    assert(s.getTypeCount() == 1);
  }


  /*
    setTypeCount
   */ 

  {
    srcMLState s;
    s.setTypeCount(4);
    assert(s.getTypeCount() == 4);
  }

  return 0;
}
