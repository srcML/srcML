while ( i-- > 0 ) 
{
    x = f( i );
    if ( x == 1 ) {
        continue;
    }
    y += x * x;
}