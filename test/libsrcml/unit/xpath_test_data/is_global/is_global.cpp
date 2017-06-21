int foo1;

int foo2();
int foo3() { }

class Foo4;
class Foo5 { };

using foo6;
using foo7 = bar;
using namespace foo8_9;

typedef long foo10;

enum Foo11;

namespace fooNS12 {
    int bar;
    int bar();
    int bar() { }
    class bar;
    class bar { };
    using bar;
    using bar = bar;
    using namespace bar;
    typedef long bar;
    enum bar;
}

extern "C" {
    int foo13;

    int foo14();
    int foo15() { }

    class Foo16;
    class Foo17 { };

    using foo18;
    using foo19 = bar;
    using namespace foo20_21;

    typedef long foo22;

    enum Foo23;
}

namespace Foo24 {
    namespace Bar {
    }
    extern "C" {
        int bar;
        int bar();
        int bar() { }
        class bar;
        class bar { };
        using bar;
        using bar = bar;
        using namespace bar;
        typedef long bar;
        enum bar;
    }
    void something(int noise);
    void something(int noise) {
        int bar;
        class bar;
        class bar { };
        using bar;
        using bar = bar;
        using namespace bar;
        typedef long bar;
        enum bar;
    }
}

void something25(int noise) {
    int bar;
    class bar;
    class bar { };
    using bar;
    using bar = bar;
    using namespace bar;
    typedef long bar;
    enum bar;
}


