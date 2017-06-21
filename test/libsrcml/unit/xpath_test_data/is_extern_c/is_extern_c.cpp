extern "C" {
    void foo1();
    void foo2() { }
}

extern "C" void foo3();
extern "C" void foo4() { }

extern "C" something foo5;


void bar1();
void bar2() { }

void bar3();
void bar4() { }

something bar5;
