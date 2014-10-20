class foo {
    
    void bar();
    void bar(){}
    
    virtual void bar1(); // 1
    virtual void bar2(){} // 2
    
    override void bar3(); // 3
    override void bar4(){} // 4
    
    sealed void bar7();
    sealed void bar8() { } 
    
    override void bar5(); // 5
    override void bar6() { } // 6
    
    abstract void bar7(); // 7
    abstract void bar8() { } // 8
    
    virtual string bar9 { // 9
        set /* 10 */; 
        get /* 11 */;
    }
    
    override string bar11 { // 12
        set /* 13 */; 
        get /* 14 */;
    }

    abstract string bar15 { // 15
        set /* 16 */; 
        get /* 17 */;
    }
    
    sealed string bar {
        set; 
        get;
    }
    
    string bar {
        set; 
        get;
    }
    
    
    override event EventType Bar18;
    abstract event EventType Bar19;
    virtual event EventType Bar20;
    sealed event EventType Bar;
    event EventType Bar;

}

interface IFoo {
    string bar { // 21
        get /* 22 */;
        set /* 23 */;
    }
    
    void bar32(); // 24
    event EventType Bar33; // 25
}

sealed class foo {
    
    void bar();
    void bar(){}
    
    virtual void bar1();
    virtual void bar2(){}
    
    override void bar3();
    override void bar4(){}
    
    sealed void bar7();
    sealed void bar8() { } 
    
    override void bar5();
    override void bar6() { }
    
    abstract void bar7();
    abstract void bar8() { }
    
    virtual string bar13 {
        set; 
        get;
    }
    
    override string bar16 {
        set; 
        get;
    }
    
    abstract string bar18 {
        set; 
        get;
    }
    
    sealed string bar21 {
        set; 
        get;
    }
    
    string bar {
        set; 
        get;
    }
    override event EventType Bar24;
    abstract event EventType Bar25;
    virtual event EventType Bar26;
    sealed event EventType Bar28;
    event EventType Bar;
}

