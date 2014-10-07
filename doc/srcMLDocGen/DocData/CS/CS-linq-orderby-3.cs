IEnumerable<string> sortDescendingQuery =
    from w in fruits
    orderby w ascending 
    select w;  
