/*
  test_Mode.cpp

  Unit tests for Mode.

 */

#include <srcMLStateStack.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <TokenParser.hpp>

class TP : public TokenParser {

public:
  TP() {}

  virtual void startElement(int) {}
  virtual void startNoSkipElement(int) {}
  virtual void endElement(int) {}
  virtual void emptyElement(int) {}
  virtual void flushSkip() {}
  virtual int SkipBufferSize() { return 0; }
  virtual antlr::RefToken* CurrentToken() { return 0; }
  virtual void addElement(int) { }
  virtual ~TP() {}
};

int main(/*int argc, char * argv[]*/) {

  /*
    size

    anything above zero causes exception on Destructor.
   */ 

  {
    TP tp;
    srcMLStateStack s(&tp);
    assert(s.size() == 0);
  }

  return 0;
}
