using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using JetBrains.Annotations;

namespace Lunar
{
    [UsedImplicitly(ImplicitUseTargetFlags.WithMembers)]
    public class Loader
    {
        private static List<object> m_gameSystems = new();

        public static event Action OnInitialize;
        public static event Action<float> OnUpdate;

        private static void LoadAssemblies()
        {
            //Register all assemblies in the build-dotnet folder
            foreach (var assembly in Directory.GetFiles(".\\build-dotnet", "*.dll",
                SearchOption.AllDirectories))
            {
                //ignore own assembly if present
                if(assembly.EndsWith("dotnetlunar.dll")) continue;

                byte[] b = File.ReadAllBytes(assembly);
                Assembly a = Assembly.Load(b);
            }
        }
        
        private static void AddResolveStrategy()
        {
            // Custom dependency resolve strategy. This will attempt to match all currently loaded
            // assemblies to all currently available assemblies
            // including _this_ assembly which is what most of these are probably going to 
            // depend on
            AppDomain.CurrentDomain.AssemblyResolve += (sender, e) =>
            {
                foreach (var assembly in AppDomain.CurrentDomain.GetAssemblies())
                {
                    if (assembly.GetName().FullName == e.Name)
                    {
                        return assembly;
                    }
                }

                return null;
            };
        }

        private static void LoadGameSystems()
        {
            //this one requires a bit of explanation ...
            AppDomain.CurrentDomain.GetAssemblies() // First we query the entire AppDomain for 
                .AsParallel()                       // all assemblies that are currently loaded in parallel
                .SelectMany(a => a.GetTypes()) // Then we get all types from those Domains
                
                //Next we select those types that have the Attribute "GameSystem"
                .Where(type => type.GetCustomAttributes(typeof(EntryPointAttribute),true).Length > 0)
                .ForAll(type =>
                {
                    
                    // then we create instances of those types, which are immediately added to the 
                    // game-system list
                    // Note that this code should technically still run in parallel which is amazing to me
                    var instance = Activator.CreateInstance(type);
                    m_gameSystems.Add(instance);
                    
                    // we also query for the Init & Update methods of those types
                    // Naturally all of this code needs to use reflection
                    MethodInfo init_method = type.GetMethod("Init");
                    MethodInfo update_method = type.GetMethod("Update");
                    
                    // If those methods have been succesfully querried we add them to the
                    // corresponding event lists
                    if (init_method is not null)
                    {
                        OnInitialize += () => init_method.Invoke(instance, null);
                    }

                    if (update_method is not null)
                    {
                        OnUpdate += delta => update_method.Invoke(instance, new object[] {delta});
                    }
                    
                });
        }
    
        [UnmanagedCallersOnly]
        public static void Initialize()
        {
            LoadAssemblies();
            AddResolveStrategy();
            LoadGameSystems();

            OnInitialize?.Invoke();
        }

        [UnmanagedCallersOnly]
        public static void Update(float delta)
        {
            OnUpdate?.Invoke(delta);            
        }

        public static List<object> GameSystems => m_gameSystems;
    }
}