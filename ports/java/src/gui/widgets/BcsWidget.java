package gui.widgets;

import core.kernel.BcsObject;
import java.util.concurrent.locks.ReentrantLock;

public class BcsWidget extends BcsObject {
    private boolean visible = false;
    private boolean enabled = true;
    private int width = 0;
    private int height = 0;
    private int x = 0;
    private int y = 0;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsWidget() {
        this(null);
    }

    public BcsWidget(BcsWidget parent) {
        super(parent);
    }

    public void show() {
        lock.lock();
        try {
            visible = true;
        } finally {
            lock.unlock();
        }
    }

    public void hide() {
        lock.lock();
        try {
            visible = false;
        } finally {
            lock.unlock();
        }
    }

    public boolean isVisible() {
        lock.lock();
        try {
            return visible;
        } finally {
            lock.unlock();
        }
    }

    public void resize(int width, int height) {
        lock.lock();
        try {
            this.width = width;
            this.height = height;
        } finally {
            lock.unlock();
        }
    }

    public void move(int x, int y) {
        lock.lock();
        try {
            this.x = x;
            this.y = y;
        } finally {
            lock.unlock();
        }
    }

    public boolean isEnabled() {
        lock.lock();
        try {
            return enabled;
        } finally {
            lock.unlock();
        }
    }

    public void setEnabled(boolean enabled) {
        lock.lock();
        try {
            this.enabled = enabled;
        } finally {
            lock.unlock();
        }
    }
}
