volatile int i;

volatile int foo();

int bar() volatile {
    return 1;
}


typedef volatile int bar;
using bar = volatile int;

asm volatile("  addic. %0,%1,%2\n" : "=r"(res): "=r"(a),"r"(a))
