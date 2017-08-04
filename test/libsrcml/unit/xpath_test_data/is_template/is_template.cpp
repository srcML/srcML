vector<Foo1> x;

template<typename Foo2>
class NodeConcrete { };

template<typename Foo3>
class NodeConcrete<Foo4> { };

template<typename Foo5>
void foo(T x) { }

template<typename Foo6>
void foo(T x);

call<Foo7>();
