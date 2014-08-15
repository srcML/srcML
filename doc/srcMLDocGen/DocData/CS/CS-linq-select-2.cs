var productQuery =  from prod in products
                    select new { prod.Color, prod.Price };
