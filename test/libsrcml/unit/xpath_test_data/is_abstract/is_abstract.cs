abstract class Foo {
    abstract void derp();
    
    abstract string name {
        get;
        set;
    }
    
    abstract event derp eventName;
}

abstract struct Foo {
    abstract void derp();
    
    abstract string name {
        get;
        set;
    }
    
    abstract event derp eventName;
    event notAbstractEvent NotAbstractEventThingy;
}

interface IFoo {
    string aName{ 
        get;
        set;
    }
    event derp anEventName;
    void derp();
}
