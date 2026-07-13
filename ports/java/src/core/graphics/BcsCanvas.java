package core.graphics;

import java.util.concurrent.locks.ReentrantLock;
import core.kernel.BcsObject;

public class BcsCanvas extends BcsObject {
    private final int width;
    private final int height;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsCanvas(BcsObject parent, int width, int height) {
        super(parent);
        this.width = width;
        this.height = height;
    }

    public int[] getSize() {
        lock.lock();
        try {
            return new int[]{width, height};
        } finally {
            lock.unlock();
        }
    }
}
