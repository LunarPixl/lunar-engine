using System.Runtime.InteropServices;
using JetBrains.Annotations;

namespace Lunar
{
    [UsedImplicitly(ImplicitUseTargetFlags.WithMembers)]
    public class UnitTest
    {
        public delegate void PassInt(int v);
        public delegate void PassString([MarshalAs(UnmanagedType.LPStr)] string v);
    
        private static PassInt m_passIntImpl;
        private static PassString m_passStringImpl;

        public delegate int RegisterFn(PassInt intfn, PassString stringfn);
        public static int Register(PassInt intfn,PassString stringfn)
        {
            m_passIntImpl = intfn;
            intfn(10);
            m_passStringImpl = stringfn;
            stringfn("Hello World");
            return 10;
        }
    }
}