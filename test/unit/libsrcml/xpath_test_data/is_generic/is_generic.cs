class Foo1<T, U>
{ }

class Foo2<T>
    : BaseNodeMultiple<T, int>
{ }


class Foo3<T, U>
    : BaseNodeMultiple<T, U>
{
    void DoWork() { }
    void DoWork4<T>() { }
    void DoWork5<T, U>() { }
}

public delegate void Del6<T>(T item);

public delegate void Derp<X>.Del6(X item);

struct Foo7<T, U>
{ }

interface Foo8<T, U>
{ }

