class PrivateTest {
    void i();
    Type x;
    typedef int something;
    using temp = somethingelse;
    PrivateTest();
    PrivateTest() { }
    
    ~PrivateTest();
    ~PrivateTest() { }
    
    enum Enum;
    enum class EnumCls;
    enum Enum { };
    enum class EnumCls { };
    
    struct Inner;
    struct Inner { };
    class PrivateTest;
    class PrivateTest { };

};

// 16 uo to here!
struct Derp {
    private:
        void i();
        Type x;
        typedef int something;
        using temp = somethingelse;
        Derp();
        Derp() { }
        
        ~Derp();
        ~Derp() { }
        
        enum Enum;
        enum class EnumCls;
        enum Enum { };
        enum class EnumCls { };
        
        struct Inner;
        struct Inner { };
        class PrivateTest;
        class PrivateTest { };        
};
// 32 up to here

enum Enum;
enum class EnumCls;
enum Enum { };
enum class EnumCls { };
class PrivateTest;
struct Derp;


class TestSuper
    :Thingy
{ };



