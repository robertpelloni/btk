using System;
using Bcs.Core.Kernel;
using Bcs.Gui.Widgets;

namespace Bcs.Gui.Kernel
{
    public enum DeviceType
    {
        Pointer = 0,
        Keyboard,
        Touch,
        Tablet,
        NumDeviceTypes
    }

    public class BcsInputArbitrator : BcsObject
    {
        private static readonly Lazy<BcsInputArbitrator> _instance = new Lazy<BcsInputArbitrator>(() => new BcsInputArbitrator());
        public static BcsInputArbitrator Instance => _instance.Value;

        private readonly int[] _deviceCounts = new int[(int)DeviceType.NumDeviceTypes];
        private readonly object _lock = new object();

        private BcsInputArbitrator() : base(null) {}

        public int DeviceCount(DeviceType type)
        {
            lock (_lock)
            {
                if (type >= 0 && type < DeviceType.NumDeviceTypes)
                {
                    return _deviceCounts[(int)type];
                }
                return 0;
            }
        }

        public void SetDeviceCount(DeviceType type, int count)
        {
            lock (_lock)
            {
                if (type >= 0 && type < DeviceType.NumDeviceTypes)
                {
                    _deviceCounts[(int)type] = count;
                }
            }
        }

        public bool RouteEvent(BcsEvent e, BcsWidget target)
        {
            if (target != null)
            {
                return target.Event(e);
            }
            return false;
        }
    }
}
