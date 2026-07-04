using System;
using System.Collections.Generic;
using System.Threading;

namespace Bcs.Core.Kernel
{
    public class BcsEventDispatcher
    {
        private readonly Queue<BcsEvent> _eventQueue = new Queue<BcsEvent>();
        private readonly object _queueLock = new object();
        private bool _running;

        public void PostEvent(BcsEvent e)
        {
            lock (_queueLock)
            {
                _eventQueue.Enqueue(e);
                Monitor.Pulse(_queueLock);
            }
        }

        public void ProcessEvents()
        {
            while (true)
            {
                BcsEvent e = null;
                lock (_queueLock)
                {
                    if (_eventQueue.Count > 0)
                    {
                        e = _eventQueue.Dequeue();
                    }
                }

                if (e == null)
                {
                    break;
                }

                DispatchEvent(e);
            }
        }

        public void Exec()
        {
            lock (_queueLock)
            {
                _running = true;
            }

            while (true)
            {
                BcsEvent e = null;
                lock (_queueLock)
                {
                    while (_eventQueue.Count == 0 && _running)
                    {
                        Monitor.Wait(_queueLock);
                    }

                    if (!_running)
                    {
                        break;
                    }

                    e = _eventQueue.Dequeue();
                }

                DispatchEvent(e);
            }
        }

        public void Stop()
        {
            lock (_queueLock)
            {
                _running = false;
                Monitor.PulseAll(_queueLock);
            }
        }

        private void DispatchEvent(BcsEvent e)
        {
            // Route to BcsObject
        }
    }
}
