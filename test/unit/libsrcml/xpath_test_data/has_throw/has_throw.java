
// Strage case of a function declared within a function.
void foo() {
    
    void bar() { 
        throw /* 1 & 2 */;
    }
}


void foo() {
    
    class bar { 
        foo() {
            throw /* 3 & 4 */;
        }
    }
}
