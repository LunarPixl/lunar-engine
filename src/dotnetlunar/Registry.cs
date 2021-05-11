using System;
using System.Collections;
using System.Collections.Generic;

namespace Lunar
{
    public class ComponentEnumerator<T> : IEnumerable<T>
    {
        public IEnumerator<T> GetEnumerator()
        {
            int index = 0;
            while (Registry.HasForIndex<T>(index))
                yield return Registry.GetAtIndex<T>(index++);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
    
    public static class Registry
    {
        private static Dictionary<Type, List<object>> m_store = new();
        
        public static ComponentEnumerator<T> CreateEnumeration<T>()
        {
            return null;
        }

        public static T Insert<T>(T obj)
        {
            return (T) Insert(typeof(T), obj);
        }

        public static object Insert(Type t, object obj)
        {
            if (m_store.ContainsKey(t))
            {
                m_store[t].Add(obj);
            }
            else
            {
                m_store[t] = new List<object> {obj};
            }
            return obj;
        }
        public static T GetAtIndex<T>(int index)
        {
            return default;
        }

        public static bool HasForIndex<T>(int index)
        {
            return false;
        }
    }
}