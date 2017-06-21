class Employee2
{
    private string name = "FirstName, LastName";// 1

    private string GetName() //2
    {
        return name;
    }
    
    string GetName//3
    {
        private get; //4
        private set; //5
    }
    
    string GetName// 6
    {
        public get;
        set; // 7
    }
    
    private class FooCls { } // 8
    private struct FooStruct{ } // 9
    
    class FooCls2 { }  // 10
    struct FooStruct2 { } // 11
    
    event privateEvent PrivateEvent;//12
    private event privateEvent PrivateEvent;//13
}

interface IEmployee {
    string GetName();
    
    event publicEvent PublicEvent;
    private event privateEvent PrivateEvent;//14
    
    string GetName
    {
        get;
        set;
    }
}
