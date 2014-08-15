IEnumerable<string> sortAscendingQuery =
    from fruit in fruits
    orderby fruit
    select fruit;
