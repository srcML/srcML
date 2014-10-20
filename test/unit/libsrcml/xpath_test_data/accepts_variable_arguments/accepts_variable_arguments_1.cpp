void foo1_2(...);

void foo3_4(...){ }

template<typename... T6_7>
using Foo5 = Bar<T...>;

void foo();

void foo() { }

template<typename T>
using Bar = bar<T>;

template <typename T>
struct bar { };

template <typename T>
class bar { };

template <typename T>
union bar { };

template <typename T>
struct bar;

template <typename T>
class bar;

template <typename T>
union bar;

foo(args...);

