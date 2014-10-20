template<typename T>
template<typename... U1_2>
void Foo<T>::Foo3(U4... x)
{ }

template<typename... U5>
template<typename T>
void Foo<U...>::Foo(T t)
{ }

template <template<typename/* 6 */...> typename X>
union foo { };
