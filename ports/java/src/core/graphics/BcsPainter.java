package core.graphics;

import java.util.concurrent.locks.ReentrantLock;
import core.kernel.BcsObject;

public class BcsPainter extends BcsObject {
    private BcsCanvas canvas;
    private boolean active;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsPainter(BcsObject parent) {
        super(parent);
        this.active = false;
    }

    public boolean begin(BcsCanvas canvas) {
        lock.lock();
        try {
            if (active) return false;
            this.canvas = canvas;
            this.active = true;
            return true;
        } finally {
            lock.unlock();
        }
    }

    public void end() {
        lock.lock();
        try {
            this.active = false;
            this.canvas = null;
        } finally {
            lock.unlock();
        }
    }
}
