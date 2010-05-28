import java.util.LinkedHashSet;


public class CompositeElement extends Element{
	
	private LinkedHashSet<Element> elements;
	
	public CompositeElement(String name)
	{
		super(name);
		elements = new LinkedHashSet<Element>();
	}
	
	public LinkedHashSet<Element> getElements()
	{
		return elements;
	}

}
