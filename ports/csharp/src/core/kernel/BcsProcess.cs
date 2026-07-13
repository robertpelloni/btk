using System;
using System.Diagnostics;

namespace Bcs.Core.Kernel
{
    public class BcsProcess : BcsObject
    {
        private Process _process;
        private readonly object _lock = new object();

        public BcsProcess(BcsObject parent = null) : base(parent)
        {
        }

        public void Start(string command, string args = "")
        {
            lock (_lock)
            {
                _process = new Process
                {
                    StartInfo = new ProcessStartInfo
                    {
                        FileName = command,
                        Arguments = args,
                        UseShellExecute = false,
                        CreateNoWindow = true
                    }
                };
                _process.Start();
            }
        }

        public void Terminate()
        {
            lock (_lock)
            {
                if (_process != null && !_process.HasExited)
                {
                    _process.Kill();
                }
            }
        }

        public void Wait()
        {
            Process p;
            lock (_lock)
            {
                p = _process;
            }
            p?.WaitForExit();
        }
    }
}
