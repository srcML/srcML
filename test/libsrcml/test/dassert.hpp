#ifndef INCLUDED_DASSERT_HPP
#define INCLUDED_DASSERT_HPP

#include <iostream>
#include <cassert>
#define dassert(LEFT, RIGHT) if(!(LEFT == RIGHT)) {\
std::cerr << "'" << LEFT << "'" << "\n";\
std::cerr << "'" << RIGHT << "'" << "\n";\
assert(LEFT == RIGHT);\
}

#endif
