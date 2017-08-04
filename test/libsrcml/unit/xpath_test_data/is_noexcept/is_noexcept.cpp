void foo1() noexcept;
void foo2() noexcept { }

void foo3() throw();
void foo4() throw() { }

void foo() throw(std::runtime_error) { }
void foo() throw(std::runtime_error);

void foo();
void foo(){ }
