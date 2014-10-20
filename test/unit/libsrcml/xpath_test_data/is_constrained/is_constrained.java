public <U extends Number> void inspect(U u) {
}

public void inspect(Bar u) {
}

public static void process(List<? extends Foo> list) { }


public static void addNumbers(List<? super Integer> list) {
}

class Box<T extends Foo> { 
}
