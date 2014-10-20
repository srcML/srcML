sealed class Foo1 {
    sealed void foo2();
    sealed void foo3() { }
    
    sealed event bar foo4;
    
    sealed string foo5 {
        get /* 6 */;
        set /* 7 */;
    }
}
