public class Employee2 // 1
{
    public string name = "FirstName, LastName"; // 2

    public string GetName() //3
    {
        return name;
    }
    
    
    public class FooCls{ } //4
    
    class FooCls2 { }
}

class Employee2
{
}

public interface IEmployee { // 5
    string GetName(); // 6
}

interface IEmployee2 { // 7
    string GetName2(); // 8
}
