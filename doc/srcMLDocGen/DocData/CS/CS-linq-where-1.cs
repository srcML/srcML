var res = from num in numbers
            where num < 5
            where num % 2 == 0
            select num;
