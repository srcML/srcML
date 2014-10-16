const int i;

const int foo();

int bar() const {
    return 1;
}

using bar = const int;

typedef const int bar;
