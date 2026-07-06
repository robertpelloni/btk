using System;

namespace Bcs.Core.Kernel
{
    public class BcsEventLoop : BcsObject
    {
        public BcsEventDispatcher EventDispatcher { get; }
        private readonly object _lock = new object();
        private bool _isRunning = false;
        private int _quitCode = 0;

        public BcsEventLoop(BcsEventDispatcher dispatcher, BcsObject parent = null) : base(parent)
        {
            EventDispatcher = dispatcher ?? new BcsEventDispatcher();
        }

        public int Exec()
        {
            lock (_lock)
            {
                if (_isRunning) return -1;
                _isRunning = true;
            }

            EventDispatcher.Exec();

            lock (_lock)
            {
                _isRunning = false;
                return _quitCode;
            }
        }

        public void Exit(int returnCode = 0)
        {
            lock (_lock)
            {
                _quitCode = returnCode;
            }
            EventDispatcher.Stop();
        }

        public void Quit()
        {
            Exit(0);
        }

        public bool IsRunning
        {
            get { lock (_lock) { return _isRunning; } }
        }
    }
}
