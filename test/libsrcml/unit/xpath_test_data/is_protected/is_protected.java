protected class Employee2 // 1
{
    protected string name = "FirstName, LastName"; //2

    protected string GetName() //3
    {
        return name;
    }
    
    
    protected class FooCls{ }  //4
    
    class FooCls2 { } 
}

class Employee2
{
}

protected interface IEmployee {
    protected string GetName();
}
