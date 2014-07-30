HelloWorld spanishGreeting = new HelloWorld() {
	String name = "mundo";
	public void greet() {
	    greetSomeone("mundo");
	}
	public void greetSomeone(String someone) {
    	name = someone;
    	System.out.println("Hola, " + name);
	}
};