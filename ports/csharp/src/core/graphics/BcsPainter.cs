using System;
using Bcs.Core.Kernel;

namespace Bcs.Core.Graphics
{
    public class BcsPainter : BcsObject
    {
        private BcsCanvas _canvas;
        private bool _active;
        private readonly object _lock = new object();

        public BcsPainter(BcsObject parent = null) : base(parent)
        {
            _active = false;
        }

        public bool Begin(BcsCanvas canvas)
        {
            lock (_lock)
            {
                if (_active) return false;
                _canvas = canvas;
                _active = true;
                return true;
            }
        }

        public void End()
        {
            lock (_lock)
            {
                _active = false;
                _canvas = null;
            }
        }
    }
}
