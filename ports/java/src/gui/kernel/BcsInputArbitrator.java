package gui.kernel;

import core.kernel.BcsEvent;
import core.kernel.BcsObject;
import gui.widgets.BcsWidget;
import java.util.concurrent.locks.ReentrantLock;

public class BcsInputArbitrator extends BcsObject {

    public enum DeviceType {
        Pointer,
        Keyboard,
        Touch,
        Tablet,
        NumDeviceTypes
    }

    private static volatile BcsInputArbitrator instance;
    private final int[] deviceCounts = new int[DeviceType.NumDeviceTypes.ordinal()];
    private final ReentrantLock lock = new ReentrantLock();

    private BcsInputArbitrator() {
        super(null);
    }

    public static BcsInputArbitrator getInstance() {
        if (instance == null) {
            synchronized (BcsInputArbitrator.class) {
                if (instance == null) {
                    instance = new BcsInputArbitrator();
                }
            }
        }
        return instance;
    }

    public int deviceCount(DeviceType type) {
        lock.lock();
        try {
            int ordinal = type.ordinal();
            if (ordinal >= 0 && ordinal < deviceCounts.length) {
                return deviceCounts[ordinal];
            }
            return 0;
        } finally {
            lock.unlock();
        }
    }

    public void setDeviceCount(DeviceType type, int count) {
        lock.lock();
        try {
            int ordinal = type.ordinal();
            if (ordinal >= 0 && ordinal < deviceCounts.length) {
                deviceCounts[ordinal] = count;
            }
        } finally {
            lock.unlock();
        }
    }

    public boolean routeEvent(BcsEvent event, BcsWidget target) {
        if (target != null) {
            return target.event(event);
        }
        return false;
    }
}
