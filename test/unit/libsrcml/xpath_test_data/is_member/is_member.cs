class Foo {
    class Foo1;     /* 1 */
    struct Foo2;    /* 2 */
    Foo();          /* 3 */
    Foo() { }       /* 4 */
    ~Foo();         /* 5 */
    ~Foo() { }      /* 6 */
    void foo7() { } /* 7 */
    void foo8();    /* 8 */
    string foo9 {   /* 9 */
        get         /* 10 */;
        set         /* 11 */;
    }
    event EventType foo12; /* 12 */
    interface Foo14 /* 13 */{ } 
    class Foo15     /* 14 */{ } 
    struct Foo16    /* 15 */{ } 
}

class Foo1;
struct Foo2;
void foo7() { }
void foo8();
string foo9 {
    get;
    set;
}
event EventType foo12;
