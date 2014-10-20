class Derp<T, U, V>{
    public void method(params something variable) {
    }
    
    Derp(params something variable);
    Derp(int a, int b, int c, params something variable);
    Derp(int a, int b, int c, params something variable) {
        x = delegate(int a, int b, int c, params something variable) { };
    }
}

delegate void DelegateType<T>(params something variable);


