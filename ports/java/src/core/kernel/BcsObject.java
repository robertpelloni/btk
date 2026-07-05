package core.kernel;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.locks.ReentrantLock;

public class BcsObject implements AutoCloseable {
    private BcsObject parent;
    private final List<BcsObject> children = new CopyOnWriteArrayList<>();
    private final ReentrantLock lock = new ReentrantLock();
    private String objectName = "";
    private boolean isDestroyed = false;

    public BcsObject() {
        this(null);
    }

    public BcsObject(BcsObject parent) {
        if (parent != null) {
            setParent(parent);
        }
    }

    public String getObjectName() {
        lock.lock();
        try {
            return objectName;
        } finally {
            lock.unlock();
        }
    }

    public void setObjectName(String objectName) {
        lock.lock();
        try {
            this.objectName = objectName;
        } finally {
            lock.unlock();
        }
    }

    public BcsObject getParent() {
        lock.lock();
        try {
            return parent;
        } finally {
            lock.unlock();
        }
    }

    public void setParent(BcsObject newParent) {
        lock.lock();
        try {
            if (parent != null && parent != newParent) {
                parent.removeChild(this);
            }
            parent = newParent;
            if (parent != null) {
                parent.addChild(this);
            }
        } finally {
            lock.unlock();
        }
    }

    private void addChild(BcsObject child) {
        children.addIfAbsent(child);
    }

    private void removeChild(BcsObject child) {
        children.remove(child);
    }

    public boolean event(BcsEvent e) {
        if (e.getType() == BcsEvent.EventType.Timer) {
            timerEvent((BcsTimerEvent) e);
            return true;
        }
        return false;
    }

    protected void timerEvent(BcsTimerEvent e) { }
    protected void childEvent(BcsChildEvent e) { }

    @Override
    public void close() {
        lock.lock();
        try {
            if (isDestroyed) return;
            isDestroyed = true;

            for (BcsObject child : children) {
                child.close();
            }
            children.clear();
            setParent(null);
        } finally {
            lock.unlock();
        }
    }
}
