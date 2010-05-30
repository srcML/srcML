/**
   srcMLDTDGenerator.java

   SAX handler to Generate DTD for srcML

   Michael Decker
   mjd52@uakron.edu
*/

import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.SAXException;
import org.xml.sax.Attributes;

import java.io.PrintWriter;
import java.io.File;
import java.io.IOException;
import java.util.Stack;
import java.util.LinkedHashSet;

public class srcMLDTDGenerator extends DefaultHandler {
	
	private PrintWriter pw;
	private Stack<String> lastelement;
	private LinkedHashSet<CompositeElement> elements;
	
	public srcMLDTDGenerator()
	{
		try{
		pw  = new PrintWriter(new File("TestDTD.txt"));
		}
		catch(IOException e)
		{
			System.err.println(e.getMessage());
		}
		
		lastelement = new Stack<String>();
		elements = new LinkedHashSet<CompositeElement>();
	}

    /**
       Notification of the start of a document
    */
    public void startDocument() 
    {}

    /**
       Notification of the end of a document
    */
    public void endDocument() 
    {
    	for(CompositeElement outer : elements)
    	{
    		pw.print(outer.getName() + ": ");
    		for(CompositeElement inner : outer.getElements())
    		{
    			pw.print(inner.getName() + " ");
    		}
    		pw.println();
    	}
    	pw.close();
    }

    /**
       Notification of the start of an element

       @param uri Namespace uri of the element.  Empty string if no namespace
       @param localName The local name (without prefix)
       @param qName The qualified name (with prefix)
       @param attributes The attributes of the element
    */
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException 
    {
		CompositeElement element = new CompositeElement(qName);
		elements.add(element);
		if(!lastelement.isEmpty())
		{
			String parent = lastelement.peek();
			for(CompositeElement e : elements)
			{
				if(e.getName().equals(parent))
				{
					e.getElements().add(element);
					break;
				}
			}
		}
		lastelement.push(qName);
    }

    /**
       Notification of the end of an element

       @param uri Namespace uri of the element.  Empty string if no namespace
       @param localName The local name (without prefix)
       @param qName The qualified name (with prefix)
    */
    public void endElement(String uri, String localName, String qName) throws SAXException 
    {
    	lastelement.pop();
    }
    
    public void characters(char[] ch, int start, int length) throws SAXException
    {
    	String inside = (new String(ch, start, length)).trim();
    	if(!inside.equals(""))
    	{
    		CompositeElement element = new CompositeElement("PCDATA");
			String parent = lastelement.peek();
			for(CompositeElement e : elements)
			{
				if(e.getName().equals(parent))
				{
					e.getElements().add(element);
					break;
				}
			}
    	}
    }
}
