class Employee2
{
    public string name = "FirstName, LastName";// 1

    public string GetName() //2
    {
        return name;
    }
    
    public string GetName//3
    {
        public get; //4
        public set; //5
    }
    
    public string GetName// 6
    {
        protected get;
        set; // 7
    }
    
    public class FooCls { } // 8
    public struct FooStruct{ } // 9
    
    class FooCls2 { }
    struct FooStruct2 { }
    
    event privateEvent PrivateEvent;
    public event privateEvent PrivateEvent;// 10
}

public interface IEmployee {// 11
    string GetName(); // 12
    
    event publicEvent PublicEvent; // 13
    public event privateEvent PrivateEvent; //14
    
    string GetName // 15
    {
        get; // 16
        set; // 17
    }
}
