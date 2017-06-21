class foo {
    
    void bar();
    void bar(){}
    
    
    virtual void bar1(); // 1
    virtual void bar2(){} // 2
    
    override void bar3(); // 3
    override void bar4(){} // 4
    
    sealed void bar7(); // 7
    sealed void bar8() { } // 8
    
    override void bar9(); // 9
    override void bar10() { } // 10
    
    abstract void bar11(); // 11
    abstract void bar12() { } // 12
    
    virtual string bar13 { // 13
        set /* 14 */; 
        get /* 15 */;
    }
    
    override string bar16 { // 16
        set /* 17 */; 
        get /* 18 */;
    }
    
    abstract string bar22 { // 22
        set /* 23 */; 
        get /* 24 */;
    }
    
    sealed string bar21 { // 25
        set /* 26 */; 
        get /* 27 */;
    }
    
    string bar {
        set; 
        get;
    }
    
    
    override event EventType Bar28;
    abstract event EventType Bar29;
    virtual event EventType Bar30;
    sealed event EventType Bar32;
    event EventType Bar;

}

interface IFoo {
    string bar33 { // 33
        get /* 34 */;
        set /* 35 */;
    }
    
    void bar36(); // 36
    event EventType Bar37; // 37
}
