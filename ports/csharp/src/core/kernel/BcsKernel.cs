using System;
using System.Collections.Generic;

namespace Bcs.Core.Kernel
{
    public interface IBcsModule
    {
        void Initialize();
        void Start();
        void Shutdown();
        string Name { get; }
    }

    public class BcsKernel : BcsObject
    {
        private static readonly Lazy<BcsKernel> _instance = new Lazy<BcsKernel>(() => new BcsKernel());
        public static BcsKernel Instance => _instance.Value;

        private readonly List<IBcsModule> _modules = new List<IBcsModule>();
        public BcsEventLoop EventLoop { get; }

        private bool _isStarted = false;
        private bool _isShuttingDown = false;
        private readonly object _lock = new object();

        private BcsKernel() : base(null)
        {
            EventLoop = new BcsEventLoop(new BcsEventDispatcher(), this);
        }

        public void RegisterModule(IBcsModule module)
        {
            lock (_lock)
            {
                if (!_isStarted)
                {
                    _modules.Add(module);
                }
            }
        }

        public void Startup()
        {
            lock (_lock)
            {
                if (_isStarted) return;
                _isStarted = true;
            }

            // Copy to avoid locks during callbacks
            List<IBcsModule> modulesCopy;
            lock (_lock) { modulesCopy = new List<IBcsModule>(_modules); }

            foreach (var mod in modulesCopy)
            {
                Console.WriteLine($"BcsKernel: Initializing module {mod.Name}");
                mod.Initialize();
            }

            foreach (var mod in modulesCopy)
            {
                Console.WriteLine($"BcsKernel: Starting module {mod.Name}");
                mod.Start();
            }
        }

        public int Exec()
        {
            try
            {
                Startup();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"BcsKernel Startup Error: {ex.Message}");
                return -1;
            }

            Console.WriteLine("BcsKernel: Entering main event loop...");
            int code = EventLoop.Exec();

            Shutdown();
            return code;
        }

        public void Shutdown()
        {
            lock (_lock)
            {
                if (_isShuttingDown) return;
                _isShuttingDown = true;
            }

            Console.WriteLine("BcsKernel: Shutting down...");

            List<IBcsModule> modulesCopy;
            lock (_lock) { modulesCopy = new List<IBcsModule>(_modules); }

            for (int i = modulesCopy.Count - 1; i >= 0; i--)
            {
                var mod = modulesCopy[i];
                Console.WriteLine($"BcsKernel: Shutting down module {mod.Name}");
                try { mod.Shutdown(); } catch { /* Ignore cleanup errors */ }
            }

            Dispose();
        }
    }
}
