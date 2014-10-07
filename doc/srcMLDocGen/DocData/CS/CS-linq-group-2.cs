var studentQuery2 =
    from student in students
    group student by student.Last[0] into g
    orderby g.Key
    select g;
