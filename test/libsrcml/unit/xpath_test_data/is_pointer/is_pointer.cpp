int* x1 /* 1, 2 & 3 */;
int* foo2() /* 4 */;
int* foo3()/* 5 */ { }
int const *x4/* 6, 7 & 8 */;
int *pointers5[10] /* 9, 10 & 11 */;

int(*X12)(void) /* 12 */;
int(*X13)(int, float) /* 13 */;

template<typename T14>
class Foo<T14 /*14*/*>  {};

template<typename T15>
class Foo< std::vector<T15 /*15*/ *> >{ };

int foo(int* foo); /* 16, 17 & 18 */

int x;
int foo();
int foo() { }
int const x;
int notpointers[10];;
