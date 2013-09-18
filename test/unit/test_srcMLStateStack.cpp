/*
  test_srcMLStateStackStack.cpp

  Unit tests for srcMLStateStack.

 */

#include <srcMLStateStack.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <TokenParser.hpp>

class TP : TokenParser {

public:
  TP() {}

  virtual void startElement(int) {}
  virtual void startNoSkipElement(int) {}
  virtual void endElement(int) {}
  virtual void emptyElement(int) {}
  virtual void flushSkip() {}
  virtual int SkipBufferSize() {}
  virtual antlr::RefToken* CurrentToken() {}
  virtual ~TP() {}
};

int main(int argc, char * argv[]) {

  /*
    size
   */ 

  {
    TP tp;
    //srcMLStateStack s(&tp);
    //assert(s.size() == 0);
  }

  {
    TP tp;
    //srcMLStateStack s(&tp);
    //s.startNewMode(1);
    //assert(s.size() == 1);
  }

  return 0;
}
