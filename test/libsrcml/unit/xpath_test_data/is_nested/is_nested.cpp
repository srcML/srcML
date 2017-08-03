
// ASM Testing 3
__asm {
    __asm mov al, 2
    __asm mov dx, 0xD007
    __asm out dx, al
};

__asm__("movl %ebx, %eax");


// Typedef 6
// Not nested
typedef int td_bar;

extern "C" {
    typedef int td_bar;
}

// Nested.
namespace { typedef int td_foo1; }
struct s{ typedef int td_foo2; };
class s{ typedef int td_foo3; };
void bar()  { typedef int td_foo4; }
class s { s() { typedef int td_foo5; } ~s() { typedef int td_foo6; } };



