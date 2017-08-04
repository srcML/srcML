    public static implicit operator double(Digit d)
    {
        return d.val;
    }
    
    public static implicit operator Digit(double d)
    {
        return new Digit(d);
    }
    
    public static implicit operator double(Digit d);
    public static implicit operator Digit(double d);
