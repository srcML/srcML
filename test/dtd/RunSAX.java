/**
   RunSAX.java

   Runs a SAX handler on an XML file.

   The name of the SAX handler class is specified on the command line,
   followed by the name of the file.

   Since the SAX handler class is called by its name (not by a constructor call),
   make sure the SAX handler class is compiled.

   Michael L. Collard
   collard@uakron.edu
*/

import java.io.FileReader;
import org.xml.sax.XMLReader;
import org.xml.sax.InputSource;
import org.xml.sax.helpers.XMLReaderFactory;
import org.xml.sax.helpers.DefaultHandler;

public class RunSAX {

    public static void main(String args[]) {

        if (args.length < 2) {

            System.err.println("usage:  java RunSax <saxhandler> <xml_input_file>");
            return;
        }

        try {

            // create the XML handler
            XMLReader xr = XMLReaderFactory.createXMLReader();

            // create a handler based on the command line argument
            Class theClass  = Class.forName(args[0]);
            DefaultHandler handler = (DefaultHandler) theClass.newInstance();
            xr.setContentHandler(handler);
            xr.setErrorHandler(handler);

            // parse the file on the command line
            xr.parse(new InputSource(new FileReader(args[1])));

        } catch (java.lang.ClassNotFoundException e) {
            System.err.println(e);

        } catch (java.lang.InstantiationException e) {
            System.err.println(e);

        } catch (java.lang.IllegalAccessException e) {
            System.err.println(e);

        } catch (java.io.FileNotFoundException e) {
            System.err.println(e);

        } catch (java.io.IOException e) {
            System.err.println(e);

        } catch (org.xml.sax.SAXException se) {
            System.err.println(se);
        }
    }
}
