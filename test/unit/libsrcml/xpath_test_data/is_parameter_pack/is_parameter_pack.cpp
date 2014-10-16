template<typename T14> class Foo<T14*>{ };

template<typename... T1>
class Foo<
    std::vector<T2...*>
>
    : Bar<T3...>
{ };

int printf(const char*, /* 4*/...);

template<typename T, typename... Args>
int printf(const char*, Args... args);
