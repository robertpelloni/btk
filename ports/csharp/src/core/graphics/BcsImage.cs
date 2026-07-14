using System;
using Bcs.Core.Kernel;

namespace Bcs.Core.Graphics
{
    public class BcsImage : BcsObject
    {
        private int _width;
        private int _height;
        private byte[] _pixels;
        private readonly object _lock = new object();

        public BcsImage(BcsObject parent, int width, int height) : base(parent)
        {
            _width = width;
            _height = height;
            _pixels = new byte[width * height * 4]; // RGBA
        }

        public (int, int) Size()
        {
            lock (_lock)
            {
                return (_width, _height);
            }
        }

        public void SetPixel(int x, int y, byte r, byte g, byte b, byte a)
        {
            lock (_lock)
            {
                if (x >= 0 && x < _width && y >= 0 && y < _height)
                {
                    int idx = (y * _width + x) * 4;
                    _pixels[idx] = r;
                    _pixels[idx + 1] = g;
                    _pixels[idx + 2] = b;
                    _pixels[idx + 3] = a;
                }
            }
        }

        public byte[] Pixels()
        {
            lock (_lock)
            {
                byte[] copy = new byte[_pixels.Length];
                Array.Copy(_pixels, copy, _pixels.Length);
                return copy;
            }
        }
    }
}
