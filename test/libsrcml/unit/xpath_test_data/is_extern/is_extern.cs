class PlatformInvokeTest
{
    [DllImport("msvcrt.dll")]
    public static extern int puts([MarshalAs(UnmanagedType.LPStr)] string m);
    [DllImport("msvcrt.dll")]
    internal static extern int _flushall();
    
}
