class Employee2
{
    protected string name = "FirstName, LastName"; // 1

    protected string GetName() // 2
    {
        return name;
    }
    
    protected internal string GetName() // 3
    {
        return name;
    }
    
    protected string GetName // 4
    {
        protected get; // 5
        protected set; // 6
    }
    
    protected string GetName // 7
    {
        private get;
        set;// 8
    }
    
    protected class FooCls{ } // 9
    protected struct FooStruct{ } // 10
    
    class FooCls2 { } 
    struct FooStruct2 { }
    
    event privateEvent PrivateEvent;
    protected event protectedEvent ProtectedEvent; // 11
}

interface IEmployee {
    string GetName();
    
    event publicEvent PublicEvent;
    protected event protectedEvent ProtectedEvent; //12
    
    string GetName
    {
        get;
        set;
    }
}
