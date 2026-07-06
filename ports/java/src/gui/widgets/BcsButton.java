package gui.widgets;

import core.kernel.BcsEvent;
import java.util.concurrent.locks.ReentrantLock;

public class BcsButton extends BcsWidget {
    private String text = "";
    private Runnable clickListener;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsButton(BcsWidget parent) {
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

    public void setClickListener(Runnable listener) {
        lock.lock();
        try {
            this.clickListener = listener;
        } finally {
            lock.unlock();
        }
    }

    @Override
    protected void mouseReleaseEvent(BcsEvent e) {
        super.mouseReleaseEvent(e);
        Runnable listener;
        lock.lock();
        try {
            listener = this.clickListener;
        } finally {
            lock.unlock();
        }

        if (listener != null) {
            listener.run();
        }
    }
}
