[assembly:AssemblyVersionAttribute("4.3.2.1")]

[assembly:AssemblyCultureAttribute("de")]


[Serializable]
public abstract class Attribute : _Attribute {
    [ignore] int val; 
}


[IsTested]
public void AddOrder(Order orderToAdd)
{
    orders.Add(orderToAdd);
}


[IsTested]
public string AddOrder
{
    set;
    get;
}

[IsTested]
public event clickevent onClick;


public override void Promote([CareerPath("Management")] Level level) { }
