using System;
using System.Runtime.InteropServices;
using JetBrains.Annotations;
using Lunar;

namespace LunarExt
{
    [EntryPoint]
    public class NativeFunctions
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int NativeMathFnI_T(int lhs, int rhs);
        public static NativeMathFnI_T NativeAddI;
        public static NativeMathFnI_T NativeSubI;
        public static NativeMathFnI_T NativeMulI;
        public static NativeMathFnI_T NativeDivI;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr GetBufferFn_T();
        public static GetBufferFn_T GetBuffer;
        

        [UsedImplicitly]
        public void Init()
        {
            NativeAddI = NativeFunctionsLibrary.GetNativeFunction<NativeMathFnI_T>("lunar::engine::test::native_add_i");
            NativeSubI = NativeFunctionsLibrary.GetNativeFunction<NativeMathFnI_T>("lunar::engine::test::native_sub_i");
            NativeMulI = NativeFunctionsLibrary.GetNativeFunction<NativeMathFnI_T>("lunar::engine::test::native_mul_i");
            NativeDivI = NativeFunctionsLibrary.GetNativeFunction<NativeMathFnI_T>("lunar::engine::test::native_div_i");
            GetBuffer =  NativeFunctionsLibrary.GetNativeFunction<GetBufferFn_T>("lunar::engine::test::native_get_buffer");
        }
    }
}