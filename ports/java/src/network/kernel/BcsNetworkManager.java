package network.kernel;

import core.kernel.BcsObject;
import java.util.concurrent.locks.ReentrantLock;

public class BcsNetworkManager extends BcsObject {
    private static volatile BcsNetworkManager instance;
    private BcsNetworkProxy applicationProxy;
    private final ReentrantLock lock = new ReentrantLock();

    private BcsNetworkManager() {
        super(null);
    }

    public static BcsNetworkManager getInstance() {
        if (instance == null) {
            synchronized (BcsNetworkManager.class) {
                if (instance == null) {
                    instance = new BcsNetworkManager();
                }
            }
        }
        return instance;
    }

    public void setApplicationProxy(BcsNetworkProxy proxy) {
        lock.lock();
        try {
            this.applicationProxy = proxy;
        } finally {
            lock.unlock();
        }
    }

    public BcsNetworkProxy getApplicationProxy() {
        lock.lock();
        try {
            return applicationProxy;
        } finally {
            lock.unlock();
        }
    }
}
