class foo {
    virtual void bar1(); // 1
    void bar();

    virtual void bar2() { } // 2
    void bar() { }
    
    void bar3() final { }
    void bar4() final;
    
    void bar3() override { }
    void bar4() override;
    
    ~foo();
    ~foo() { }

    virtual ~foo();
    virtual ~foo() { }
};

class Derp final {
    public:
        virtual void bar1();
        void bar();

        virtual void bar2() { }
        void bar() { }
        
        void bar3() final { }
        void bar4() final;
        
        void bar5() override { }
        void bar6() override;
        
        ~foo();
        ~foo() { }

        virtual ~foo();
        virtual ~foo() { }  
};
