event EventHandler IDrawingObject.OnDraw
{
    add
    {
        PreDrawEvent += value;
    }
    remove
    {
        PreDrawEvent -= value;
    }
}
