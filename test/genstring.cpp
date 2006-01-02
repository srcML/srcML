#include <iostream>

void output(std::ostream& out) {

  unsigned char c = 9;
  out << c;
  c = 11;
  out << c;
  c = 12;
  //  out << c;

  for (c = 32; c < 127; ++c)
    out << c;
}

int main() {

  std::cout << "//";
  output(std::cout);
  std::cout << '\n';

  std::cout << "/*";
  output(std::cout);
  std::cout << "*/";
  std::cout << '\n';

  return 0;
}
