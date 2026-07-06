using System;

namespace Bcs.Gui.Widgets
{
    public class BcsLabel : BcsWidget
    {
        private string _text = string.Empty;
        private readonly object _lock = new object();

        public BcsLabel(BcsWidget parent) : base(parent)
        {
        }

        public string Text
        {
            get { lock (_lock) { return _text; } }
            set { lock (_lock) { _text = value; } }
        }
    }
}
