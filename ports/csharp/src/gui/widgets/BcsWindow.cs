using System;

namespace Bcs.Gui.Widgets
{
    public class BcsWindow : BcsWidget
    {
        private string _title = string.Empty;
        private readonly object _lock = new object();

        public BcsWindow() : base(null)
        {
        }

        public string Title
        {
            get { lock (_lock) { return _title; } }
            set { lock (_lock) { _title = value; } }
        }
    }
}
