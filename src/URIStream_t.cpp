#include "URIStream.h"
#include <iostream>
#include <vector>
#include <cstring>

int main() {

  URIStream stream("/dev/stdin");

  char* line;
  while (line = stream.getline())
    std::cout << "|" << line << "|" << '\n';
}
