#if (DEBUG && !MYTEST)
        Console.WriteLine("DEBUG is defined");
#else
        Console.WriteLine("DEBUG and MYTEST are not defined");
#endif
