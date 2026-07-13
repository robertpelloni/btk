using System;
using Bcs.Core.Kernel;

namespace Bcs.Core.Graphics
{
    public class BcsCanvas : BcsObject
    {
        private int _width;
        private int _height;
        private readonly object _lock = new object();

        public BcsCanvas(BcsObject parent, int width, int height) : base(parent)
        {
            _width = width;
            _height = height;
        }

        public (int, int) Size()
        {
            lock (_lock)
            {
                return (_width, _height);
            }
        }
    }
}
