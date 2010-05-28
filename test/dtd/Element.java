import java.util.LinkedHashSet;
public class Element{
	private String name;
	
	public Element(String name)
	{
		this.name = name;
	}
	
	public String getName()
	{
		return name;
	}
	
	public boolean equals(Object e)
	{
		return name.equals(((Element)e).name);
	}
	
	public int hashCode()
	{
		return name.hashCode();
	}
}
