class Foo {
public:
    Foo() = default;
    ~Foo() = default;
    Foo& operator=(Foo const&) = default;
    void bar();
    void bar(){}
    
};


