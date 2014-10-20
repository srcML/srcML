private class Employee2 // 1
{
    private string name = "FirstName, LastName"; // 2

    private string GetName() //3
    {
        return name;
    }
    
    
    private class FooCls{ } //4
    
    class FooCls2 { } //5
}

class Employee2 // 6
{
}

interface IEmployee // 7
{
    string GetName();
}
