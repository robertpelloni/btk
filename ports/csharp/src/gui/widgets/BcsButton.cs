using System;

namespace Bcs.Gui.Widgets
{
    public class BcsButton : BcsWidget
    {
        private string _text = string.Empty;
        private readonly object _lock = new object();

        public event Action Clicked;

        public BcsButton(BcsWidget parent) : base(parent)
        {
        }

        public string Text
        {
            get { lock (_lock) { return _text; } }
            set { lock (_lock) { _text = value; } }
        }

        protected override void MouseReleaseEvent(Core.Kernel.BcsEvent e)
        {
            base.MouseReleaseEvent(e);
            Clicked?.Invoke();
        }
    }
}
