using System;
using Bcs.Core.Kernel;

namespace Bcs.Gui.Widgets
{
    public class BcsWidget : BcsObject
    {
        private bool _visible = false;
        private bool _enabled = true;
        private int _width = 0;
        private int _height = 0;
        private int _x = 0;
        private int _y = 0;
        private readonly object _lock = new object();

        public BcsWidget(BcsWidget parent = null) : base(parent)
        {
        }

        public void Show()
        {
            lock (_lock) { _visible = true; }
        }

        public void Hide()
        {
            lock (_lock) { _visible = false; }
        }

        public bool IsVisible
        {
            get { lock (_lock) { return _visible; } }
        }

        public void Resize(int width, int height)
        {
            lock (_lock)
            {
                _width = width;
                _height = height;
            }
        }

        public void Move(int x, int y)
        {
            lock (_lock)
            {
                _x = x;
                _y = y;
            }
        }

        public bool IsEnabled
        {
            get { lock (_lock) { return _enabled; } }
            set { lock (_lock) { _enabled = value; } }
        }
    }
}
