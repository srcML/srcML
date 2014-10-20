class Foo {
    
    public void SampleMethod(out int i) { }
    
    static void main() {
        int value;
        string str1, str2;
        Method(out value, out str1, out str2);
    }
}

interface ICovariant<out R> { }
