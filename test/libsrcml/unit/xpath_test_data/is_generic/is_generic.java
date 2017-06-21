static <T> void Foo1(T[] a, Collection<T> c) {
}

static Collection<T> Foo() {
}


interface Foo2<E> {
    public <T> boolean foo3(Collection<T> c);
    public <T extends E> boolean foo4(Collection<T> c);
}

class Foo5<T> {
}
