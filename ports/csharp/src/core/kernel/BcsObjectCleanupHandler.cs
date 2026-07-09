using System.Collections.Generic;

namespace Bcs.Core.Kernel
{
    public class BcsObjectCleanupHandler : BcsObject
    {
        private readonly List<BcsObject> _cleanupObjects = new List<BcsObject>();
        private readonly object _lock = new object();

        public BcsObjectCleanupHandler() : base(null)
        {
        }

        public BcsObject Add(BcsObject obj)
        {
            lock (_lock)
            {
                if (!_cleanupObjects.Contains(obj))
                {
                    _cleanupObjects.Add(obj);
                }
            }
            return obj;
        }

        public void Remove(BcsObject obj)
        {
            lock (_lock)
            {
                _cleanupObjects.Remove(obj);
            }
        }

        public bool IsEmpty()
        {
            lock (_lock)
            {
                return _cleanupObjects.Count == 0;
            }
        }

        public void Clear()
        {
            List<BcsObject> objectsToClear;
            lock (_lock)
            {
                objectsToClear = new List<BcsObject>(_cleanupObjects);
                _cleanupObjects.Clear();
            }

            foreach (var obj in objectsToClear)
            {
                obj.Dispose();
            }
        }
    }
}
