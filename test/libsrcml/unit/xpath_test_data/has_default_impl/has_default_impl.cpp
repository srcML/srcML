class Foo {
    Foo() = default;
    ~Foo() = default;
    Foo& operator=(Foo const&) = default;
};

class Bar {
    Bar();
    ~Bar();
    Bar& operator=(Bar const&);
};
