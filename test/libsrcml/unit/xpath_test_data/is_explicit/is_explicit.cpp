class C 
{
public:
    int i;
    explicit C(const C&)   // an explicit copy constructor
    {
        printf_s("\nin the copy constructor");
    }
    explicit C(int i )   // an explicit constructor
    {
        printf_s("\nin the constructor");
    }
    explicit operator int() const { return 0; }
}
