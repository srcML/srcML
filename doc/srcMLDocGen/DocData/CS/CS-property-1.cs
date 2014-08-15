public double Hours
{
    get { return seconds / 3600; }
    set { seconds = value * 3600; }
}
