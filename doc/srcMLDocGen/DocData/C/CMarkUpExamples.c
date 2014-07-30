


/* C - Example code */

// Generic
_Generic ( AnyExpression , int:5, double:5.0, default:0);

// Array access
x[5];
matrix[x][y];

// Call Examples.
int x = atoi("50");

// Member access examples
struct tm t;
int seconds = t.tm_sec;

// Pointer Member Access
struct tm* t;
int seconds = t->tm_sec;

// Extract Operators from Here
identifier++;
identifier--;
++identifier;
--identifier;
sizeof identifier;
sizeof(struct type);
_Alignof(union type);
&identifier;
*identifier;
-identifier;
~identifier;
!identifier;

// Cast Example
double x = 0.0;
int y = (int)x;

// Other operators
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

// Comma operator
expr1, expr2, expr3;

// Static assert example
_Static_assert(DEBUG_LEVEL > 5, "Message");

// Declarations
// Declaring a primitive variable
int x;

// Declaring a struct, union or enum variable.
struct X y;
union X y;
enum X y;

// Declaring a variable using _Alignas 
_Alignas(struct X) struct X y;
_Alignas(64) int y[4];

// Declaring an _Atomic variable.
_Atomic union X t;
_Atomic int t;
_Atomic(int) t;

// Declaring a const variable.
const int x;
const enum X y;
int const x;
enum const X y;

// Declaring an extern variable or function.
extern int X;
extern struct Z X;
extern void foo();

// Declaring an extern variable or function with linkage specifier.
extern "C" void foo();
extern "C" struct Z X;
extern "C" int X;

// Declaring an extern block
extern "C" { }

// Declaring a thread local variable.
_Thread_local int x;
_Thread_local union Z x;

// Declaring an auto variable
auto int x;

// Declaring a static function or variable.
static int x;
static Z y;
static void foo();

// Declaring a typedef examples
typedef char C;
typedef unsigned int WORD;
typedef char * pChar;
typedef char field [50]; 
typedef char field, *field2; 
typedef struct club 
{
    char name[30];
    int size, year;
} GROUP;
// Declaring a type of a function.
typedef void DRAWF( int, int );

// Declaring a function pointer type.
typedef void (*DRAWF)( int, int );

// Declaring a register variable example.

// register examples
register int x;
register float y;

// restrict examples.
restrict *double x;
restrict *union Z x;

// volatile examples
volatile int foo; 
int volatile foo;

// _Noreturn function
_Noreturn void die(const char *fmt, ...) { exit(EXIT_FAILURE); }

// Inline function example
inline void swapValues(int* a,int* b);



// Declaring structs, unions and enums.

// Struct and Variable of that type at the same time.
struct a {
    int x;
    struct b
    {
      int y;
    } var2;
} var1;

// More instance variables.
struct a var3;
struct b var4;

// Anonymous structure declarations
struct
{
    int x;
    int y;
} mystruct;

// Defines a structure variable named temp
struct employee
{
    char name[20];
    int id;
    long class;
} temp;

// Declaring multiple variables of the struct type at the same time.
struct employee student, faculty, staff;

// Declaring struct bit fields.
struct mybitfields
{
    unsigned short a : 4;
    unsigned short b : 5;
    unsigned short c : 7;
} test;

// Struct initialization Examples.
typedef struct MY_TYPE {
  boolean flag;
  short int value;
  double stuff;
} MY_TYPE;

MY_TYPE a = { .flag = true, .value = 123, .stuff = 0.456 };
a = (MY_TYPE) { false, 234, 1.234 };
a = { false, 234, 1.234 };



// Declaring a union (Can be done the same ways as a struct, just without the bitfields).
union sign   /* A definition and a declaration */
{
    int svar;
    unsigned uvar;
} number;

// Defines a two-dimensional
// array named screen 
union {
    struct { 
        unsigned int icon : 8;  
        unsigned color : 4;
    } window1;
    int screenval;
} screen[25][80];




// enum declarations examples
enum DAY            /* Defines an enumeration type    */
{
    saturday,       /* Names day and declares a       */
    sunday = 0,     /* variable named workday with    */ 
    monday,         /* that type                      */
    tuesday,
    wednesday,      /* wednesday is associated with 3 */
    thursday,
    friday
} workday;

enum DAY today = wednesday;

// Ways one can declare the the variables end_flag and match flag.
// Declares an enumeration data type called BOOLEAN 
enum BOOLEAN
{
    false,
    true 
}; 

// Two variables of type BOOLEAN
// Example 1
enum BOOLEAN end_flag, match_flag;

// Example 2
enum BOOLEAN { false, true } end_flag, match_flag;

// Example 3
enum BOOLEAN { false, true } end_flag;
enum BOOLEAN match_flag;

// enum cast example.
workday = ( enum DAY ) ( day_value - 1 );

// Anonymous enum
enum { yes, no } response;




// Array Declaration examples
char A[2][3];
float matrix[10][15];
struct {
    float x, y;
} complex[100];
extern char *name[];

// Array Initialization Example
int myArray[10] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
int myArray[10] = { 1, 2 }; //initialize to 1,2,0,0,0...
int myArray[10] = { 0 }; // all elements 0
int a[6] = { [4] = 29, [2] = 15 };




// Pointer declaration examples.
// Declares a pointer variable named message 
char *message;

// Declares an array of pointers 
int *pointers[10];

// Declares a pointer to an array of 10 elements 
int (*pointer)[10];

// Declares a pointer variable, x,to a constant value 
int const *x;

// CV-qualifier examples.
const int some_object = 5 ;
int other_object = 37;
int *const y = &fixed_object;
int volatile *const z = &some_object;
int *const volatile w = &some_object;

// Uses the tag for list
struct list *next, *previous;

// pointer within the type they point to.
struct list 
{
    char *token;
    int count;
    struct list *next;
} line;

// variable named x that is a pointer to a function
int(*X)(void);
int(*X)(int, float);
int(*X)(char const*,...);






// Abstract declarator examples: (Advanced topic)
// The type name for a pointer to type int:
int *;

// An array of three pointers to int
int *[3];

// A pointer to an array of five int
int (*) [5];

// A function with no parameter specification
// returning a pointer to int
int *();


// A pointer to a function taking no arguments and 
// returning an int 
int (*) ( void );

// An array of an unspecified number of constant pointers to 
// functions each with one parameter that has type unsigned int 
// and an unspecified number of other parameters returning an int 
int (*const []) ( unsigned int, ... );



// Function Declaration Examples

// Struct return type example
struct STUDENT sortstu( STUDENT a, STUDENT b );
void print(STUDENT a);

// Array parameter
void foo (int a[]);

// Pointer parameter
void foo (int* a);

// varargs Function Parameter Declaration examples
int add_multiple_values (int number, ...);


// Function Definition Examples
// K&R C parameters example
int func(a, b, c)
   int a;
   int b;
   int c;
{ }

struct STUDENT sortstu( STUDENT a, STUDENT b ) {
    struct STUDENT x;
    return x;
}

void print(STUDENT a) { {

// Array parameter
void foo (int a[]) { } 

// Pointer parameter
void foo (int* a) { }

// varargs Function Parameter Declaration examples
int add_multiple_values (int number, ...) { }




// Statement Examples (A-Z).
// break Statement
char c;
for(;;) {
    printf_s( "\nPress any key, Q to quit: " );

    // Convert to character value
    scanf_s("%c", &c);
    if (c == 'Q')
        break;
}


// Compound Statement/Block
if ( i > 0 ) {
    line[i] = x;
    x++;
    i--;
}

// continue Statement
while ( i-- > 0 ) 
{
    x = f( i );
    if ( x == 1 ) {
        continue;
    }
    y += x * x;
}


// do-while Statement
do {
    y = f( x );
    x--;
} while ( x > 0 );


// Expression Statement
x = ( y + 3 );            /* x is assigned the value of y + 3  */
x++;                      /* x is incremented                  */
x = y = 0;                /* Both x and y are initialized to 0 */
proc( arg1, arg2 );       /* Function call returning void      */
y = z = ( f( x ) + 3 );   /* A function-call expression        */


// for Statement
for( ;; ) { }

for (i = 0; i < max; i++ ) 
{
  if ( line[i] == ' ' )
  {
      space++;
  }
  if ( line[i] == '\t' )
  {
      tab++;
  }
}




// goto and Labeled Statements
for ( i = 0; i < 10; i++ )
{
    printf_s( "Outer loop executing. i = %d\n", i );
    for ( j = 0; j < 3; j++ )
    {
        printf_s( " Inner loop executing. j = %d\n", j );
        if ( i == 5 )
            goto stop;
    }
}

/* This message does not print: */
printf_s( "Loop exited. i = %d\n", i );

stop:
printf_s( "Jumped to stop. i = %d\n", i );


// if Statement
if ( i > 0 )
    y = x / i;
else 
{
    x = i;
    y = f( x );
}

if ( i > 0 )           /* Without braces */
    if ( j > i )
        x = j;
    else
        x = i;

if ( i > 0 ) 
{                      /* With braces */
    if ( j > i )
        x = j;
}
else
    x = i;
        

// Null Statement/Empty Statement
for ( i = 0; i < 10; line[i++] = 0 ) ;


// return Statement
long long sq( int s )
{
    // Note that parentheses around the return expression 
    // are allowed but not required here.
    return( s * (long long)s );
}

void draw( int i, long long ll )
{
    printf( "i = %d, ll = %lld\n", i, ll );
    return;
}


// switch Statement
switch( c ) 
{
    case 'A':
        capa++;
    case 'a':
        lettera++;
    default :
        total++;
}

switch( i ) 
{
    case -1:
        n++;
        break;
    case 0 :
        z++;
        break;
    case 1 :
        p++;
        break;
}

// while Statement
while ( i >= 0 ) 
{
    string1[i] = string2[i];
    i--;
}


