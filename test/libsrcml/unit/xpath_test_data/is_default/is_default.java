
public interface Interface { 
    default Bar Foo1();
    default Bar Foo() {
    }
}

public interface Interface { 
    string Foo1() default "Something";
}
