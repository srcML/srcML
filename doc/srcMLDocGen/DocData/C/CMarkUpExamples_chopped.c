
// Do this last maybe?
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


// continue Statement



// do-while Statement



// Expression Statement
x = ( y + 3 );            /* x is assigned the value of y + 3  */
x++;                      /* x is incremented                  */
x = y = 0;                /* Both x and y are initialized to 0 */
proc( arg1, arg2 );       /* Function call returning void      */
y = z = ( f( x ) + 3 );   /* A function-call expression        */



// if Statement



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



// while Statement


