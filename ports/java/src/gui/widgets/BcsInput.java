package gui.widgets;

import java.util.concurrent.locks.ReentrantLock;

public class BcsInput extends BcsWidget {
    private String text = "";
    private final ReentrantLock lock = new ReentrantLock();

    public BcsInput(BcsWidget parent) {
        super(parent);
    }

    public String getText() {
        lock.lock();
        try {
            return text;
        } finally {
            lock.unlock();
        }
    }

    public void setText(String text) {
        lock.lock();
        try {
            this.text = text;
        } finally {
            lock.unlock();
        }
    }
}
