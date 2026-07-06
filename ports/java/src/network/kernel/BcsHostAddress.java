package network.kernel;

import core.kernel.BcsObject;
import java.util.concurrent.locks.ReentrantLock;

public class BcsHostAddress extends BcsObject {
    private String address;
    private boolean isIPv6;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsHostAddress(String address) {
        super(null);
        this.address = address;
    }

    @Override
    public String toString() {
        lock.lock();
        try {
            return address;
        } finally {
            lock.unlock();
        }
    }
}
