class Foo1 {
    int data;
    friend std::ostream& operator<<(std::ostream& out, const Y& o);
    friend char* X::foo(int);
    friend X::X(char), X::~X();
    friend SomeType;
};

union Foo2 {
    friend char* X::foo(int);
    friend X::X(char), X::~X();
    friend T;

};

struct Foo3 {
    friend T;
};

class Bar1 { } ;
union Bar2 { } ;
struct Bar3 { } ;
