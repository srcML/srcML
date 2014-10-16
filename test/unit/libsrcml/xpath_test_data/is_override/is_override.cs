class Square : ShapesClass
{
    int side = 0;

    public Square(int n)
    {
        side = n;
    }

    public override int Area()
    {
        return side * side;
    }
}
