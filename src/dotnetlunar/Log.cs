using System;
using System.Runtime.InteropServices;
using JetBrains.Annotations;

namespace Lunar
{
    [PublicAPI("Logging API")]
    public class Logger
    {
        public enum LogSeverity
        {
            Trace = 0,
            Debug = 1,
            Info = 2,
            Warning = 3,
            Error = 4,
            Panic = 5
        }

        public delegate void LogFN(int severity, [MarshalAs(UnmanagedType.LPStr)] string msg);

        private static LogFN m_fn;

        public delegate void ReceivedLogDelegateFn(LogFN fn);

        private static void ReceivedLogDelegate(LogFN fn)
        {
            m_fn = fn;
        }

        public static void Log(LogSeverity severity, string msg)
        {
            m_fn((int) severity, msg);
        }

        public static void Trace(string msg) => Log(LogSeverity.Trace, msg);

        public static void Debug(string msg) => Log(LogSeverity.Debug, msg);
        public static void Info(string msg) => Log(LogSeverity.Info, msg);
        public static void Warning(string msg) => Log(LogSeverity.Warning, msg);
        public static void Error(string msg) => Log(LogSeverity.Error, msg);
        public static void Panic(string msg) => Log(LogSeverity.Panic, msg);
    }
}