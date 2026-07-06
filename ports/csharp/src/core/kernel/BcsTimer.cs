using System;
using System.Threading;
using System.Threading.Tasks;

namespace Bcs.Core.Kernel
{
    public class BcsTimer : BcsObject
    {
        private static int _timerIdCounter = 0;
        private int _intervalMs = 0;
        private bool _singleShot = false;
        private bool _isActive = false;
        private readonly int _timerId;
        private CancellationTokenSource _cts;
        private readonly object _lock = new object();

        public event Action Timeout;

        public BcsTimer(BcsObject parent = null) : base(parent)
        {
            _timerId = Interlocked.Increment(ref _timerIdCounter);
        }

        public int Interval
        {
            get { lock (_lock) { return _intervalMs; } }
            set { lock (_lock) { _intervalMs = value; } }
        }

        public bool SingleShot
        {
            get { lock (_lock) { return _singleShot; } }
            set { lock (_lock) { _singleShot = value; } }
        }

        public bool IsActive
        {
            get { lock (_lock) { return _isActive; } }
        }

        public int TimerId => _timerId;

        public void Start(int msec = -1)
        {
            if (msec > 0)
            {
                Interval = msec;
            }

            Stop();

            lock (_lock)
            {
                _isActive = true;
                _cts = new CancellationTokenSource();
            }

            RunLoopAsync(_cts.Token);
        }

        public void Stop()
        {
            lock (_lock)
            {
                if (!_isActive) return;
                _isActive = false;
                _cts?.Cancel();
                _cts?.Dispose();
                _cts = null;
            }
        }

        private async void RunLoopAsync(CancellationToken token)
        {
            try
            {
                int delay = Interval;
                bool isSingle = SingleShot;

                if (isSingle)
                {
                    await Task.Delay(delay, token);
                    Timeout?.Invoke();
                    DispatchTimerEvent();
                    lock (_lock) { _isActive = false; }
                }
                else
                {
                    while (!token.IsCancellationRequested)
                    {
                        await Task.Delay(delay, token);
                        Timeout?.Invoke();
                        DispatchTimerEvent();
                    }
                }
            }
            catch (TaskCanceledException)
            {
                // Expected when stopped
            }
        }

        private void DispatchTimerEvent()
        {
            var e = new BcsTimerEvent(_timerId);
            BcsKernel.Instance.EventLoop.EventDispatcher.PostEvent(e);
        }
    }

    public class BcsCoreApplication : BcsObject
    {
        private static readonly Lazy<BcsCoreApplication> _instance = new Lazy<BcsCoreApplication>(() => new BcsCoreApplication());
        public static BcsCoreApplication Instance => _instance.Value;

        private BcsCoreApplication() : base(null) {}

        public int Exec()
        {
            return BcsKernel.Instance.Exec();
        }

        public void Quit()
        {
            BcsKernel.Instance.Shutdown();
        }
    }
}
