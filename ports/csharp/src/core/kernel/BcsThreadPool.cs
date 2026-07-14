using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;

namespace Bcs.Core.Kernel
{
    public class BcsThreadPool : BcsObject
    {
        private BlockingCollection<Action> _tasks;
        private List<Thread> _workers;
        private CancellationTokenSource _cts;
        private bool _isShutdown;
        private readonly object _lock = new object();

        public BcsThreadPool(BcsObject parent, int numWorkers) : base(parent)
        {
            _tasks = new BlockingCollection<Action>();
            _cts = new CancellationTokenSource();
            _workers = new List<Thread>(numWorkers);
            _isShutdown = false;

            for (int i = 0; i < numWorkers; i++)
            {
                var thread = new Thread(WorkerLoop);
                thread.IsBackground = true;
                thread.Start();
                _workers.Add(thread);
            }
        }

        private void WorkerLoop()
        {
            // Drain tasks correctly without OperationCanceledException immediately stopping it
            foreach (var task in _tasks.GetConsumingEnumerable())
            {
                try
                {
                    task();
                }
                catch
                {
                    // Log or handle individual task exceptions
                }
            }
        }

        public void Submit(Action task)
        {
            lock (_lock)
            {
                if (!_isShutdown)
                {
                    _tasks.Add(task);
                }
            }
        }

        public void Shutdown()
        {
            lock (_lock)
            {
                if (_isShutdown) return;
                _isShutdown = true;
                _tasks.CompleteAdding(); // Stops taking new, allows GetConsumingEnumerable to finish naturally
            }

            // Wait for workers to finish outside the lock
            foreach (var worker in _workers)
            {
                worker.Join();
            }
        }
    }
}
