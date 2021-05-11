using Lunar;

namespace LunarExt
{
    [EntryPoint]
    public class VerboseOutput : LunarSystem
    {
        public void Init()
        {
            
            Logger.Info("GameSystem Initialization Phase Started");
            OrderedExecutor.SurpriseAdd(Whaddup,ProcessOrdering.Last);

            var res = NativeFunctions.NativeMulI(5, 5);
            res = NativeFunctions.NativeAddI(res, 5);
            res = NativeFunctions.NativeDivI(res, 2);
            res = NativeFunctions.NativeSubI(res,10);

            using (var r = new WrapArray(NativeFunctions.GetBuffer()))
            {
                Logger.Info($"Acquired Resource: {r}");
            }
            
            Logger.Info($"{res.ToString()}");
            Logger.Info("fn exists");

        }
        
        public void Whaddup()
        {
            Logger.Info("Whaddup");
        }

        [ExecuteWhen(ProcessOrdering.Before)]
        public void OrderedUpdateBefore()
        {
            Logger.Info("Before");
        }

        [ExecuteWhen(ProcessOrdering.Standard)]
        public void OrderedUpdate()
        {
            Logger.Info($"I was updated at {ProcessOrdering.Standard}");
        }

    }
}