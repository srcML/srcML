struct S {
    S(const std::string& arg) try : m(arg) {
    } catch(const std::exception& e) {
    } // implicit throw; here
    
    S() { }
    ~S()
    try {

    } catch(...) {

    }
};

int f(int n)
try {

} catch(...) {

}


int f(int n) { }
