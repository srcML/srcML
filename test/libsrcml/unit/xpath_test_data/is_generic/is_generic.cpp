

template<typename T> class Foo1;
template<typename T> class Foo2 { };

template<typename T> struct Foo3;
template<typename T> struct Foo4 { };

template<typename T> union Foo5;
template<typename T> union Foo6 { };

template<typename T> void Foo7();
template<typename T> void Foo8() { };


template<typename T>
using Foo9 = something<T>;

class Foo1;
class Foo2 { };

struct Foo3;
struct Foo4 { };

union Foo5;
union Foo6 { };

void Foo7();
void Foo8() { };

using Foo = int;

