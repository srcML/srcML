class foo {
    foo() {
        x = 0;
    }
    void foo_1() {
        x = 0;
    }
    
    string foo_1() {
        get;
        set;
    }
    event FooEventType Event;
    
    ~foo() {
        x = 0;
    }
}


unsafe {
    x = 0;
}
