import java.util.LinkedHashSet;


public class CompositeElement{
	
	private String name;
	private LinkedHashSet<CompositeElement> elements;
	
	public CompositeElement(String name)
	{
		this.name = name;
		elements = new LinkedHashSet<CompositeElement>();
	}
	
	public String getName()
	{
		return name;
	}
	
	public LinkedHashSet<CompositeElement> getElements()
	{
		return elements;
	}
	
	public boolean equals(Object e)
	{
		return name.equals(((CompositeElement)e).name);
	}
	
	public int hashCode()
	{
		return name.hashCode();
	}
}
