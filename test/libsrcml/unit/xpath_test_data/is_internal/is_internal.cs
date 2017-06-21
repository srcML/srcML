class Employee2//1
{
    internal string name = "FirstName, LastName";// 2

    internal string GetName() //3
    {
        return name;
    }
    
    internal string GetName//4
    {
        internal get; //5
        internal set; //6
    }
    
    internal string GetName// 7
    {
        protected get;
        set; // 8
    }
    
    internal class FooCls { } // 9
    internal struct FooStruct{ } // 10
    
    class FooCls2 { }
    struct FooStruct2 { }
    
    event privateEvent PrivateEvent;
    internal event privateEvent PrivateEvent;// 11
}

interface IEmployee {// 12
    string GetName();
    
    event publicEvent PublicEvent;
    internal event privateEvent PrivateEvent; //13
    
    string GetName
    {
        get;
        set;
    }
}
