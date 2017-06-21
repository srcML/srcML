enum Bar;
enum Bar { };

class Bar { 
    enum Foo1 { };
    enum Foo2;
};

union Bar { 
    enum Foo3 { };
    enum Foo4;
};
struct Bar { 
    enum Foo5 { };
    enum Foo6;
};

namespace {
    enum Bar { };
    enum Bar;
}

enum class Foo7;
enum class Foo8 { };
