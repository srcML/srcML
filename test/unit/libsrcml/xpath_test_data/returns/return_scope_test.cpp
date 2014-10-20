
// Strage case of a function declared within a function.
void foo() {
    
    void bar() { 
        return /* 1 & 2 */;
    }
}


void foo() {
    
    class bar { 
        foo() {
            return /* 3 & 4 */;
        }
    };
}

void foo() {
    
    x = [](){ return/* 5 & 6 */; };
}
