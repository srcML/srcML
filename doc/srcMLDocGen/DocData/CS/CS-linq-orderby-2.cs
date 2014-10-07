IEnumerable<string> sortDescendingQuery =
    from w in fruits
    orderby w descending 
    select w;  
