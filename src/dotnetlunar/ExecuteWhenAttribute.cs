using System;
using JetBrains.Annotations;

namespace Lunar
{
    [UsedImplicitly]
    [MeansImplicitUse,AttributeUsage(AttributeTargets.Method)]
    public sealed class ExecuteWhenAttribute : Attribute
    {
        public readonly byte Ordering;

        public ExecuteWhenAttribute(byte ordering)
        {
            Ordering = ordering;
        }
    }
}