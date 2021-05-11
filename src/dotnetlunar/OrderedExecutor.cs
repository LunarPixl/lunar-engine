using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using JetBrains.Annotations;

namespace Lunar
{

    public static class OrderedExecutor
    {
        private static SortedList<byte, Action> m_ordered_processes = new();
        private static bool m_dirtyBit = false;


        public static void SurpriseAdd(Action a,byte when)
        {
            if (m_ordered_processes.ContainsKey(when))
            {
                m_ordered_processes[when] += a;
            }
            else
            {
                m_ordered_processes[when] = a;
                m_dirtyBit = true;
            }
        }
        
        [UnmanagedCallersOnly,UsedImplicitly]
        public static void Startup()
        {
            //Get all assemblies in current domain
            AppDomain.CurrentDomain.GetAssemblies().AsParallel()
                //Get all types from that domain
                .SelectMany(x => x.GetTypes())

                //check if those types inherit from LunarScript
                .Where(t => typeof(LunarSystem).IsAssignableFrom(t) &&
                            t != typeof(LunarSystem))

                //Get all methods from those types (including the type)
                .Select(t => (t, t.GetMethods()))

                //Slim down the methods to those only that have an attribute of type ExecuteWhenAttribute
                .Select(pair =>
                (
                    pair.Item1,
                    pair.Item2.Where(mi => mi.GetCustomAttributes().Any(a => a is ExecuteWhenAttribute)).ToArray())
                )

                // Remove pairs that have no applicable methods
                .Where(pair => pair.Item2.Length > 0)

                //Do something with those pairs
                .ForAll( x =>
                {
                    var (T, Infos) = x;

                    //Create instance of the Script
                    var instance = Activator.CreateInstance(T);
                    foreach (var Info in Infos)
                    {
                        //get the attribute
                        var attrib = Info.GetCustomAttribute<ExecuteWhenAttribute>();

                        //insert invocation of element into action group for correct ordering


                        if (!m_ordered_processes.ContainsKey(attrib.Ordering))
                        {
                            m_ordered_processes.Add(attrib.Ordering,null);
                        }

                        m_ordered_processes[attrib.Ordering] += () => Info.Invoke(instance,new object[]{});
                    }
                }
            );
        }

        [UnmanagedCallersOnly,UsedImplicitly]
        private static byte HasRunnersFor(byte index)
        {
            return m_ordered_processes.ContainsKey(index).ToBlittable();
        }

        [UnmanagedCallersOnly, UsedImplicitly]
        private static byte DirtyBitSet()
        {
            var bit = m_dirtyBit;
            m_dirtyBit = false;
            return bit.ToBlittable();
        }
        
        [UnmanagedCallersOnly,UsedImplicitly]
        private static void UpdateIndex(byte index)
        {
            m_ordered_processes[index]();
        }
    }
}