class Foo {
    class Foo1          /* 1 */;
    struct Foo2         /* 2 */;
    Foo()               /* 3 */;
    Foo(X const& v)     /* 4 */ { }
    ~Foo();             /* 5 */
    ~Foo()              /* 6 */ { }
    void foo7()         /* 7 */ { }
    void foo8()         /* 8 */;
    class Foo9          /* 9 */  { };
    struct Foo10        /* 10 */ { };
    union Foo11         /* 11 */ { };
    enum Foo12          /* 12 */ { };
    enum class Foo13    /* 13 */ { };
    enum class Foo14    /* 14 */;
    enum Foo15          /* 15 */;
    typedef type Foo16  /* 16 */;
    using type = Foo17  /* 17 */;
    int a               /* 18 & 19 */; 
}

class Foo1;
struct Foo2;
void foo7() { }
void foo8();

enum foo {
    a                   /* 20 */,
    b                   /* 21 */,
};
