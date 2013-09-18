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

  return 0;
}
