package core.kernel;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;

public class BcsObjectCleanupHandler extends BcsObject {
    private final List<BcsObject> cleanupObjects = new ArrayList<>();
    private final ReentrantLock lock = new ReentrantLock();

    public BcsObjectCleanupHandler() {
        super(null);
    }

    public BcsObject add(BcsObject obj) {
        lock.lock();
        try {
            if (!cleanupObjects.contains(obj)) {
                cleanupObjects.add(obj);
            }
            return obj;
        } finally {
            lock.unlock();
        }
    }

    public void remove(BcsObject obj) {
        lock.lock();
        try {
            cleanupObjects.remove(obj);
        } finally {
            lock.unlock();
        }
    }

    public boolean isEmpty() {
        lock.lock();
        try {
            return cleanupObjects.isEmpty();
        } finally {
            lock.unlock();
        }
    }

    public void clear() {
        List<BcsObject> objectsToClear;
        lock.lock();
        try {
            objectsToClear = new ArrayList<>(cleanupObjects);
            cleanupObjects.clear();
        } finally {
            lock.unlock();
        }

        for (BcsObject obj : objectsToClear) {
            obj.close();
        }
    }
}
