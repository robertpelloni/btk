package gui.widgets;

import java.util.concurrent.locks.ReentrantLock;

public class BcsWindow extends BcsWidget {
    private String title = "";
    private final ReentrantLock lock = new ReentrantLock();

    public BcsWindow() {
        super(null);
    }

    public String getTitle() {
        lock.lock();
        try {
            return title;
        } finally {
            lock.unlock();
        }
    }

    public void setTitle(String title) {
        lock.lock();
        try {
            this.title = title;
        } finally {
            lock.unlock();
        }
    }
}
