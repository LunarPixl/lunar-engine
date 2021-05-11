using System;
using JetBrains.Annotations;

namespace Lunar
{
    [PublicAPI]
    public sealed class EngineResource : IDisposable
    {
        internal EngineResource(IntPtr ptr, bool is_array)
        {
            m_resource = ptr;
            m_isArray = is_array;
        }

        private bool m_isArray;
        private IntPtr m_resource;

        public IntPtr Resource => m_resource;
        
        private delegate void NativeFreeFn(IntPtr p);
        private static NativeFreeFn NativeFreePrimitive;
        private static NativeFreeFn NativeFreeArray;
        
        
        private delegate void RegisterNativeFreeFn(NativeFreeFn primitive,
                                                   NativeFreeFn array);
        private static void RegisterNativeFree(NativeFreeFn primitive,
                                               NativeFreeFn array)
        {
            NativeFreePrimitive = primitive;
            NativeFreeArray = array;
        }


        public void Dispose()
        {
            if (m_isArray)
                NativeFreeArray(m_resource);
            else
                NativeFreePrimitive(m_resource);
        }
    }

    public sealed class WrapArray : IDisposable
    {
        private EngineResource m_resource;

        public WrapArray(IntPtr ptr)
        {
            m_resource = new EngineResource(ptr, true);
        }
        
        public void Dispose()
        {
            m_resource?.Dispose();
        }
    }

    public sealed class WrapPrimitive : IDisposable
    {
        private EngineResource m_resource;

        public WrapPrimitive(IntPtr ptr)
        {
            m_resource = new EngineResource(ptr, false);
        }
        
        public void Dispose()
        {
            m_resource?.Dispose();
        }
    }
    
}