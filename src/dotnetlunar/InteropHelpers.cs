namespace Lunar
{
    public static class BlittableBool
    {
        public const byte True = 1;
        public const byte False = 0;

        public static byte ToBlittable(this bool b)
        {
            return b ? BlittableBool.True : BlittableBool.False;
        }
    }
}