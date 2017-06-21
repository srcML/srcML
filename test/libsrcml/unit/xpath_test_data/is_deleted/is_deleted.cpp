void functionIsDeleted() = delete;
void functionIsDeleted();

class foo {
    public:
        foo() = delete;
        ~foo() = delete;
        foo();
        ~foo();
};
