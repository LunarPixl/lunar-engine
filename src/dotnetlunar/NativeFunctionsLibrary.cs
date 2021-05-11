using System;
using System.Runtime.InteropServices;
using JetBrains.Annotations;

namespace Lunar
{
    public class NativeFunctionsLibrary
    {

        public delegate IntPtr
            GetNativeFunctionFn([MarshalAs(UnmanagedType.LPStr)] string name);
        private static GetNativeFunctionFn m_getNativeFunction;

        public delegate int
            HasNativeFunctionFn([MarshalAs(UnmanagedType.LPStr)] string name);
        private static HasNativeFunctionFn m_hasNativeFunction;        

        public delegate void RegisterGetterFn(GetNativeFunctionFn fn1,HasNativeFunctionFn fn2);

        private static void RegisterGetter(GetNativeFunctionFn fn1,HasNativeFunctionFn fn2)
        {
            m_getNativeFunction = fn1;
            m_hasNativeFunction = fn2;
        }

        public static bool HasNativeFunction(in string name)
        {
            return m_hasNativeFunction(name) != 0;
        }

        public static T GetNativeFunction<T>(in string name) where T :Delegate
        {
            if(m_hasNativeFunction(name) != 0)
                return (T) Marshal.GetDelegateForFunctionPointer<T>(m_getNativeFunction(name));

            throw new ArgumentException("Function not found");
        }

    }
}