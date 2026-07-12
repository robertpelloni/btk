using System;
using Bcs.Core.Kernel;
using Bcs.Gui.Kernel;

namespace Bcs.Gui.Widgets
{
    public class BcsInput : BcsWidget
    {
        private string _text = "";
        private readonly object _lock = new object();

        public BcsInput(BcsWidget parent = null) : base(parent)
        {
        }

        public string Text
        {
            get
            {
                lock (_lock) { return _text; }
            }
            set
            {
                lock (_lock) { _text = value; }
            }
        }
    }
}
