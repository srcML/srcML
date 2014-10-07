var earlyBirdQuery =
    from sentence in strings
    let words = sentence.Split(' ')
    from word in words
    let w = word.ToLower()
    where w[0] == 'a' || w[0] == 'e'
        || w[0] == 'i' || w[0] == 'o'
        || w[0] == 'u'
    select word;
