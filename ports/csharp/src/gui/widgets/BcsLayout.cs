using System.Collections.Generic;
using Bcs.Core.Kernel;

namespace Bcs.Gui.Widgets
{
    public class BcsLayout : BcsObject
    {
        private List<BcsWidget> _widgets;
        private readonly object _lock = new object();

        public BcsLayout(BcsObject parent = null) : base(parent)
        {
            _widgets = new List<BcsWidget>();
        }

        public void AddWidget(BcsWidget widget)
        {
            lock (_lock)
            {
                _widgets.Add(widget);
            }
        }

        public void RemoveWidget(BcsWidget widget)
        {
            lock (_lock)
            {
                _widgets.Remove(widget);
            }
        }

        public List<BcsWidget> Widgets()
        {
            lock (_lock)
            {
                return new List<BcsWidget>(_widgets);
            }
        }
    }
}
