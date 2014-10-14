
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
    }
}
