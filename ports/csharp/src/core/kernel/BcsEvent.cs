using System;

namespace Bcs.Core.Kernel
{
    public enum EventType
    {
        None = 0,
        Timer,
        MouseButtonPress,
        MouseButtonRelease,
        MouseButtonDblClick,
        MouseMove,
        KeyPress,
        KeyRelease,
        FocusIn,
        FocusOut,
        Enter,
        Leave,
        Paint,
        Move,
        Resize,
        Create,
        Destroy,
        Show,
        Hide,
        Close,
        Quit,
        ApplicationActivate,
        ApplicationDeactivate,
        User = 1000
    }

    public abstract class BcsEvent
    {
        public EventType Type { get; }
        public bool IsSpontaneous { get; set; }
        private bool _accepted = true;
        private readonly object _lock = new object();

        protected BcsEvent(EventType type)
        {
            Type = type;
        }

        public bool IsAccepted
        {
            get { lock (_lock) { return _accepted; } }
            set { lock (_lock) { _accepted = value; } }
        }

        public void Accept() { IsAccepted = true; }
        public void Ignore() { IsAccepted = false; }
    }

    public class BcsTimerEvent : BcsEvent
    {
        public int TimerId { get; }

        public BcsTimerEvent(int timerId) : base(EventType.Timer)
        {
            TimerId = timerId;
        }
    }

    public class BcsChildEvent : BcsEvent
    {
        public object Child { get; } // Maps to BcsObject later

        public BcsChildEvent(EventType type, object child) : base(type)
        {
            Child = child;
        }
    }
}
