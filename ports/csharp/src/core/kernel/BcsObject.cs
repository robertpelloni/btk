using System;
using System.Collections.Generic;

namespace Bcs.Core.Kernel
{
    public class BcsObject : IDisposable
    {
        private BcsObject _parent;
        private readonly List<BcsObject> _children = new List<BcsObject>();
        private readonly object _lock = new object();
        private bool _isDisposed = false;

        public string ObjectName { get; set; } = string.Empty;

        public BcsObject(BcsObject parent = null)
        {
            if (parent != null)
            {
                SetParent(parent);
            }
        }

        public BcsObject Parent
        {
            get { lock (_lock) { return _parent; } }
        }

        public void SetParent(BcsObject newParent)
        {
            lock (_lock)
            {
                if (_parent != null && _parent != newParent)
                {
                    _parent.RemoveChild(this);
                }

                _parent = newParent;

                if (_parent != null)
                {
                    _parent.AddChild(this);
                }
            }
        }

        private void AddChild(BcsObject child)
        {
            lock (_lock)
            {
                if (!_children.Contains(child))
                {
                    _children.Add(child);
                }
            }
        }

        private void RemoveChild(BcsObject child)
        {
            lock (_lock)
            {
                _children.Remove(child);
            }
        }

        public virtual bool Event(BcsEvent e)
        {
            if (e.Type == EventType.Timer)
            {
                TimerEvent((BcsTimerEvent)e);
                return true;
            }
            return false;
        }

        protected virtual void TimerEvent(BcsTimerEvent e) { }
        protected virtual void ChildEvent(BcsChildEvent e) { }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (_isDisposed) return;

            if (disposing)
            {
                List<BcsObject> childrenCopy;
                lock (_lock)
                {
                    childrenCopy = new List<BcsObject>(_children);
                    _children.Clear();
                }

                foreach (var child in childrenCopy)
                {
                    child.Dispose();
                }

                SetParent(null);
            }

            _isDisposed = true;
        }

        ~BcsObject()
        {
            Dispose(false);
        }
    }
}
