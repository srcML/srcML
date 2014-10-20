class foo {
    virtual void bar1(); // 1
    void bar();

    virtual void bar2() { } // 2
    void bar() { }
    
    void bar3() final { } // 3
    void bar4() final;// 4
    
    void bar5() override { } // 5
    void bar6() override; // 6
    
    ~foo();
    ~foo() { }

    virtual ~foo(); // 7
    virtual ~foo() { } // 8
};


