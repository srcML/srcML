/*
  test_State.cpp

  Unit tests for Options.

 */

#include <State.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>

int main(int argc, char * argv[]) {

  /*
    pop
   */ 

  {
    State s(1);
    try{
      s.pop();
      assert(false);
    } catch(...) {}
  }

  /*
    inMode
   */ 

  {
    State s(1, 2, 4);
    assert(s.inMode(1));
  }

  {
    State s(1, 2, 4);
    assert(!s.inMode(2));
  }

  /*
    inPrevMode
   */ 

  {
    State s(1, 2, 4);
    assert(s.inPrevMode(4));
  }

  {
    State s(1, 2, 4);
    assert(!s.inPrevMode(1));
  }

  /*
    inTransparentMode
   */ 

  {
    State s(1, 2, 4);
    assert(s.inTransparentMode(3));
  }

  {
    State s(1, 2, 4);
    assert(!s.inTransparentMode(4));
  }

  /*
    getMode
   */ 

  {
    State s(1);
    assert(s.getMode() == 1);
  }

  /*
    getTransparentMode
   */ 

  {
    State s(1, 2);
    assert(s.getTransparentMode() == 3);
  }

  /*
    setMode
   */ 

  {
    State s(1, 2);
    s.setMode(4);
    assert(s.getMode() == 5);
    assert(s.getTransparentMode() == 7);
  }

  /*
    setMode
   */ 

  {
    State s(1 | 4, 2 | 8);
    s.clearMode(4);
    s.clearMode(8);
    assert(s.getMode() == 1);
    assert(s.getTransparentMode() == 3);
  }

  return 0;
}
