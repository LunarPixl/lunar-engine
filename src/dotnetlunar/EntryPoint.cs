using System;
using JetBrains.Annotations;

namespace Lunar
{
    [AttributeUsage(AttributeTargets.Class)]
    [MeansImplicitUse(ImplicitUseTargetFlags.WithMembers)]
    public class EntryPointAttribute : Attribute
    {
        
    }
}