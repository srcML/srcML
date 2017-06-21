template <T>
template <typename... T1_2>
struct Outer<T>::foo3 { };

template <typename... T4_5>
template <T>
struct Outer<T...>::foo { };
