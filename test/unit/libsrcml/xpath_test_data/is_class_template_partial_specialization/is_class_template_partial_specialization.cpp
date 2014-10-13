class Bar {};

template<typename> class Bar {};

template<typename T> class Foo1<vector<T>> { };

template<typename T> class OuterClass::Foo1<vector<T>> { };

template<> class Name<int>::Bar {};

template<> class Name::Bar<int> {};

template<> template<> class Name<int>::Bar<int> {};
