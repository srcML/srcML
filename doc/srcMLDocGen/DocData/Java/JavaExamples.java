
/* Java language examples. */

Variable Declaration examples:
boolean result = true;
char capitalC = 'C';
byte b = 100;
short s = 10000;
int i = 100000;

int cadence = 0;
int speed = 0;
int gear = 1;

int x6 = 0x5_2; 
int x3 = 5_______2;
int x1 = 5_2;
long socialSecurityNumber1 = 999_99_9999_L;
float pi2 = 3._1415F;

long creditCardNumber = 1234_5678_9012_3456L;
long socialSecurityNumber = 999_99_9999L;
float pi =  3.14_15F;
long hexBytes = 0xFF_EC_DE_5E;
long hexWords = 0xCAFE_BABE;
long maxLong = 0x7fff_ffff_ffff_ffffL;
byte nybbles = 0b0010_0101;
long bytes = 0b11010010_01101001_10010100_10010010;

// Object Examples
char data[] = {'a', 'b', 'c'};
String str = new String(data);
String cde = "cde";

// Array Examples
// declares an array of integers
int[] anArray;

// Array Initialization
char[] copyFrom = { 'd', 'e', 'c', 'a', 'f', 'f', 'e', 'i', 'n', 'a', 't', 'e', 'd' };
int[] anArray = new int[10];
int[][] multi = new int[5][10];

// Java Operators
lhsExpr * rhsExpr;
lhsExpr / rhsExpr;
lhsExpr % rhsExpr;
lhsExpr + rhsExpr;
lhsExpr - rhsExpr;
lhsExpr << rhsExpr;
lhsExpr >> rhsExpr;
lhsExpr < rhsExpr;
lhsExpr > rhsExpr;
lhsExpr <= rhsExpr;
lhsExpr >= rhsExpr;
lhsExpr == rhsExpr;
lhsExpr != rhsExpr;
lhsExpr & rhsExpr;
lhsExpr ^ rhsExpr;
lhsExpr | rhsExpr;
lhsExpr && rhsExpr;
lhsExpr || rhsExpr;
ConditionalExpr ? expr : expr;
lhsExpr = rhsExpr;
lhsExpr *= rhsExpr;
lhsExpr /= rhsExpr;
lhsExpr %= rhsExpr;
lhsExpr += rhsExpr;
lhsExpr -= rhsExpr;
lhsExpr <<= rhsExpr;
lhsExpr >>= rhsExpr;
lhsExpr &= rhsExpr;
lhsExpr ^= rhsExpr;
lhsExpr |= rhsExpr;
lhsExpr >>>= rhsExpr;
lhsExpr >>> rhsExpr;
lhsExpr instanceof rhsExpr;
// Unary operators
identifier++;
identifier--;
++identifier;
--identifier;
-identifier;
~identifier;
!identifier;


// Example Expression Statements

// assignment statement
aValue = 8933.234;

// increment statement
aValue++;

// method invocation statement
System.out.println("Hello World!");

// object creation statement
Bicycle myBike = new Bicycle();

// Block Usage Example
if (condition) { // begin block 1
    System.out.println("Condition is true.");
} // end block 1
else { // begin block 2
   System.out.println("Condition is false.");
}

// if Statements:
if (isMoving){ 
    currentSpeed--;
}
// if-else
if (isMoving) {
    currentSpeed--;
} else {
    System.err.println("The bicycle has already stopped!");
} 
    
// else-if
if (testscore >= 90) {
    grade = 'A';
} else if (testscore >= 80) {
    grade = 'B';
} else if (testscore >= 70) {
    grade = 'C';
} else if (testscore >= 60) {
    grade = 'D';
} else {
    grade = 'F';
}


// Switch statement
switch (month) {
    case 1:  monthString = "January";
             break;
    case 2:  monthString = "February";
             break;
    case 3:  monthString = "March";
             break;
    case 4:  monthString = "April";
             break;
    case 5:  monthString = "May";
             break;
    case 6:  monthString = "June";
             break;
    case 7:  monthString = "July";
             break;
    case 8:  monthString = "August";
             break;
    case 9:  monthString = "September";
             break;
    case 10: monthString = "October";
             break;
    case 11: monthString = "November";
             break;
    case 12: monthString = "December";
             break;
    default: monthString = "Invalid month";
             break;
}

switch (month) {
    case 1:  monthString = "January";
             break;
    case 2:  monthString = "February";
             break;
    case 3:  monthString = "March";
             break;
    case 4:  monthString = "April";
             break;
    case 5:  monthString = "May";
             break;
    case 6:  monthString = "June";
             break;
    case 7:  monthString = "July";
             break;
    case 8:  monthString = "August";
             break;
    case 9:  monthString = "September";
             break;
    case 10: monthString = "October";
             break;
    case 11: monthString = "November";
             break;
    case 12: monthString = "December";
             break;
    default: monthString = "Invalid month";
             break;
}

// While Statement
int count = 1;
while (count < 11) {
    System.out.println("Count is: " + count);
    count++;
}

// Do while Statement
int count = 1;
do {
    System.out.println("Count is: " + count);
    count++;
} while (count < 11);


// for statement
for(int i=1; i<11; i++){
    System.out.println("Count is: " + i);
}  
         
// infinite loop
for ( ; ; ) {
    // your code goes here
}

// Range for
for (int item : numbers) {
    System.out.println("Count is: " + item);
}

// Branch TO:
search:
    for (i = 0; i < arrayOfInts.length; i++) {
        for (j = 0; j < arrayOfInts[i].length;
             j++) {
            if (arrayOfInts[i][j] == searchfor) {
                foundIt = true;
                break search;
            }
        }
    }
    
    
// Continue from
test:
    for (int i = 0; i <= max; i++) {
        int n = substring.length();
        int j = i;
        int k = 0;
        while (n-- != 0) {
            if (searchMe.charAt(j++) != substring.charAt(k++)) {
                continue test;
            }
        }
        foundIt = true;
        break test;
    }
    
    
// Declaring/defining a class
public class Bicycle {
        
    // the Bicycle class has
    // three fields
    public int cadence;
    private int gear;
    protected int speed;
        
    // the Bicycle class has
    // one constructor
    public Bicycle(int startCadence, int startSpeed, int startGear) {
        gear = startGear;
        cadence = startCadence;
        speed = startSpeed;
    }
        
    // the Bicycle class has
    // four methods
    public void setCadence(int newValue) {
        cadence = newValue;
    }
        
    public void setGear(int newValue) {
        gear = newValue;
    }
        
    public void applyBrake(int decrement) {
        speed -= decrement;
    }
        
    public void speedUp(int increment) {
        speed += increment;
    }
        
}


// Method declarations

// Arbitrary Number of Arguments Examples
public Polygon polygonFrom(Point... corners) {
    int numberOfSides = corners.length;
    double squareOfSide1, lengthOfSide1;
    squareOfSide1 = (corners[1].x - corners[0].x)
                     * (corners[1].x - corners[0].x) 
                     + (corners[1].y - corners[0].y)
                     * (corners[1].y - corners[0].y);
    lengthOfSide1 = Math.sqrt(squareOfSide1);

    // more method body code follows that creates and returns a 
    // polygon connecting the Points
}

// Declaring objects.
Point originOne = new Point(23, 94);
Rectangle rectOne = new Rectangle(originOne, 100, 200);
Rectangle rectTwo = new Rectangle(50, 100);

// Field accessor example
System.out.println("Width of rectOne: "  + rectOne.width);
System.out.println("Height of rectOne: " + rectOne.height);

// return example
public int getArea() {
    return width * height;
}
public void foo() {
    return;
}

// this keyword (in a method):
public class Point {
    public int x = 0;
    public int y = 0;
        
    //constructor
    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }
}

// Using this keyword to call a constructor.
public class Rectangle {
    private int x, y;
    private int width, height;
        
    public Rectangle() {
        this(0, 0, 1, 1);
    }
    public Rectangle(int width, int height) {
        this(0, 0, width, height);
    }
    public Rectangle(int x, int y, int width, int height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }
    
}

// Static constant example
static final double PI = 3.141592653589793;


// Class Field initialization
public class BedAndBreakfast {

    // initialize to 10
    public static int capacity = 10;

    // initialize to false
    private boolean full = false;
}

// Static block initialization
class Whatever {
    public static varType myVar = initializeClassVariable();
        
    private static varType initializeClassVariable() {

        // initialization code goes here
    }
}

// Instance Variable initialization using a final method
class Whatever {
    private varType myVar = initializeInstanceVariable();
        
    protected final varType initializeInstanceVariable() {

        // initialization code goes here
    }
}

// Nested classes
class OuterClass {
    
    class NestedClass {
        
    }
}

class OuterClass {
    
    static class StaticNestedClass {
        
    }
    class InnerClass {
        
        
    }
}

// Interface declaration example
interface DataStructureIterator extends java.util.Iterator<Integer> { } 

// Inner class implements the DataStructureIterator interface,
// which extends the Iterator<Integer> interface    
public class EvenIterator implements DataStructureIterator {
    
    // Start stepping through the array from the beginning
    private int nextIndex = 0;
    
    public boolean hasNext() {
        
        // Check if the current element is the last in the array
        return (nextIndex <= SIZE - 1);
    }        
    
    public Integer next() {
        
        // Record a value of an even index of the array
        Integer retValue = Integer.valueOf(arrayOfInts[nextIndex]);
        
        // Get the next even element
        nextIndex += 2;
        return retValue;
    }
}

// Local class example:
 public static void validatePhoneNumber(
    String phoneNumber1, String phoneNumber2) {
   
    final int numberLength = 10;
     
    // Valid in JDK 8 and later:
    
    // int numberLength = 10;
    
    class PhoneNumber {
         
        String formattedPhoneNumber = null;

        PhoneNumber(String phoneNumber){
            // numberLength = 7;
            String currentNumber = phoneNumber.replaceAll(
              regularExpression, "");
            if (currentNumber.length() == numberLength)
                formattedPhoneNumber = currentNumber;
            else
                formattedPhoneNumber = null;
        }

        public String getNumber() {
            return formattedPhoneNumber;
        }
    }

    PhoneNumber myNumber1 = new PhoneNumber(phoneNumber1);
    PhoneNumber myNumber2 = new PhoneNumber(phoneNumber2);

    if (myNumber1.getNumber() == null) 
        System.out.println("First number is invalid");
    else
        System.out.println("First number is " + myNumber1.getNumber());
    if (myNumber2.getNumber() == null)
        System.out.println("Second number is invalid");
    else
        System.out.println("Second number is " + myNumber2.getNumber());

}

// Anonymous class example
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


// Lambda Example
printPersons(
    roster,
    (Person p) -> p.getGender() == Person.Sex.MALE
        && p.getAge() >= 18
        && p.getAge() <= 25
);
// More lambda examples
Calculator myApp = new Calculator();
IntegerMath addition = (a, b) -> a + b;
IntegerMath subtraction = (a, b) -> a - b;
System.out.println("40 + 2 = " + myApp.operateBinary(40, 2, addition));
System.out.println("20 - 10 = " + myApp.operateBinary(20, 10, subtraction));


// Method reference example
Arrays.sort(rosterAsArray, Person::compareByAge);


// Enum example
public class EnumTest {
    Day day;
    
    public EnumTest(Day day) {
        this.day = day;
    }
    
    public void tellItLikeItIs() {
        switch (day) {
            case MONDAY:
                System.out.println("Mondays are bad.");
                break;
                    
            case FRIDAY:
                System.out.println("Fridays are better.");
                break;
                         
            case SATURDAY: case SUNDAY:
                System.out.println("Weekends are best.");
                break;
                        
            default:
                System.out.println("Midweek days are so-so.");
                break;
        }
    }
}


public enum Planet {
    MERCURY (3.303e+23, 2.4397e6),
    VENUS   (4.869e+24, 6.0518e6),
    EARTH   (5.976e+24, 6.37814e6),
    MARS    (6.421e+23, 3.3972e6),
    JUPITER (1.9e+27,   7.1492e7),
    SATURN  (5.688e+26, 6.0268e7),
    URANUS  (8.686e+25, 2.5559e7),
    NEPTUNE (1.024e+26, 2.4746e7);

    private final double mass;   // in kilograms
    private final double radius; // in meters
    Planet(double mass, double radius) {
        this.mass = mass;
        this.radius = radius;
    }
    private double mass() { return mass; }
    private double radius() { return radius; }

    // universal gravitational constant  (m3 kg-1 s-2)
    public static final double G = 6.67300E-11;

    double surfaceGravity() {
        return G * mass / (radius * radius);
    }
    double surfaceWeight(double otherMass) {
        return otherMass * surfaceGravity();
    }
    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("Usage: java Planet <earth_weight>");
            System.exit(-1);
        }
        double earthWeight = Double.parseDouble(args[0]);
        double mass = earthWeight/EARTH.surfaceGravity();
        for (Planet p : Planet.values())
           System.out.printf("Your weight on %s is %f%n",
                             p, p.surfaceWeight(mass));
    }
}

// Annotation  Example

@Override
void mySuperMethod() {}

@Entity

@Author(
   name = "Benjamin Franklin",
   date = "3/27/2003"
)
class MyClass() { }

@SuppressWarnings("unchecked")
void myMethod() { }

@Author(name = "Jane Doe")
@Author(name = "John Smith")
class MyClass { }


// Declaring an annotation
@interface ClassPreamble {
   String author();
   String date();
   int currentRevision() default 1;
   String lastModified() default "N/A";
   String lastModifiedBy() default "N/A";
   // Note use of array
   String[] reviewers();
}


// Interface examples.

public interface GroupedInterface extends Interface1, Interface2, Interface3 {
    // constant declarations
    
    // base of natural logarithms
    double E = 2.718282;
 
    // method signatures
    void doSomething (int i, double x);
    int doSomethingElse(String s);
}


// Interface implements keyword example.
public interface Relatable {
        
    // this (object calling isLargerThan)
    // and other must be instances of 
    // the same class returns 1, 0, -1 
    // if this is greater than, 
    // equal to, or less than other
    public int isLargerThan(Relatable other);
}

public class RectanglePlus 
    implements Relatable { }
    

// Interface static & default methods. 
public interface TimeClient {
    void setTime(int hour, int minute, int second);
    void setDate(int day, int month, int year);
    void setDateAndTime(int day, int month, int year,
                               int hour, int minute, int second);
    LocalDateTime getLocalDateTime();
    
    static ZoneId getZoneId (String zoneString) {
        try {
            return ZoneId.of(zoneString);
        } catch (DateTimeException e) {
            System.err.println("Invalid time zone: " + zoneString +
                "; using default time zone instead.");
            return ZoneId.systemDefault();
        }
    }
        
    default ZonedDateTime getZonedDateTime(String zoneString) {
        return ZonedDateTime.of(getLocalDateTime(), getZoneId(zoneString));
    }
}


// Inheritance example (extends keyword)
public class MountainBike extends Bicycle { }

// Abstract class & method example:
public abstract class GraphicObject {

   abstract void draw();
}

// Super keyword example
public class Subclass extends Superclass {

    // overrides printMethod in Superclass
    public void printMethod() {
        super.printMethod();
    }
}

// Final method example.
class ChessAlgorithm {
    final ChessPlayer getFirstPlayer() {
        return ChessPlayer.WHITE;
    }
}

// Generic class example
/**
 * Generic version of the Box class.
 * @param <T> the type of the value being boxed
 */
public class Box<T> {
    // T stands for "Type"
    private T t;

    public void set(T t) { this.t = t; }
    public T get() { return t; }
}

// Declaring a variable with a generic type
Box<Integer> integerBox;

// Java Diamond example
Box<Integer> integerBox = new Box<>();


// Generic Method example
public class Util {
    // Generic static method
    public static <K, V> boolean compare(Pair<K, V> p1, Pair<K, V> p2) {
        return p1.getKey().equals(p2.getKey()) &&
               p1.getValue().equals(p2.getValue());
    }
}


// Generic Bound parameter example
public <U extends Number> void inspect(U u){
    System.out.println("T: " + t.getClass().getName());
    System.out.println("U: " + u.getClass().getName());
}


// Extending generic bound parameters
public static <T extends Comparable<T>> int countGreaterThan(T[] anArray, T elem) {
    int count = 0;
    for (T e : anArray)
        if (e.compareTo(elem) > 0)
            ++count;
    return count;
}

// Upper Bounded Wildcard generic parameter example
public static void process(List<? extends Foo> list) { }

// Unbound wildcard
public static void printList(List<?> list) {
    for (Object elem: list)
        System.out.print(elem + " ");
    System.out.println();
}

// Lower Bounded  wildcard
public static void addNumbers(List<? super Integer> list) {
    for (int i = 1; i <= 10; i++) {
        list.add(i);
    }
}

// Creating a package
package graphics;

// Import example
import org.collections.*;



// ThrowStatement
throw myExceptionVariable;

// SynchronizedStatement
synchronized(itemToSyncOn) { }

// TryStatement
// 'try' Block Catches
try { } catch(Exception varName) { }
try { } catch(Exception varName) { } catch(Exception2 varName) { }
try { } catch(Exception varName) { } catch(Exception2 varName) { }
    catch(Exception3 varName) { }

// The following demonstrates the possible parameter list
// for a catch.
try { } catch(final Exception varName) { }
try { } catch(@Annotation final Exception varName) { }
try { } catch(@Annotation Exception varName) { }
try { } catch(Exception | Exception2 | Exception3 | Exception4 varName) { }

// try block + optional Catches + Finally Block 
try { } finally { } 
try { } catch (Exception e) { } finally { } 
try { } catch (Exception e) { } catch (Exception2 e) { } finally { } 

// TryWithResourcesStatement :
// 'try' ResourceSpecification Block ?Catches ?Finally
try (final res1 = expr1; res2 = expr2; @Annotation res3 = expr3) { }
try (final res1 = expr1; res2 = expr2; @Annotation res3 = expr3;) { }
try (res1 = expr1;) { }
try (res1 = expr1) { }

// Resource list + catches + finally.
try (res1 = expr1){ } finally { } 
try (res1 = expr1){ } catch (Exception e) { } finally { } 
try (res1 = expr1){ } catch (Exception e) { } catch (Exception2 e) { } finally { } 

// Methods with exception specification (throws)
public MyOtherType foo() throws MyExceptionType { }
public MyOtherType foo() throws MyExceptionType throws MyExceptionType2 { }
public MyOtherType foo() throws MyExceptionType, MyExceptionType2 { }
public MyOtherType foo() throws { }
