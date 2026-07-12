package gui.widgets;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;
import core.kernel.BcsObject;

public class BcsLayout extends BcsObject {
    private final List<BcsWidget> widgets;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsLayout(BcsObject parent) {
        super(parent);
        this.widgets = new ArrayList<>();
    }

    public void addWidget(BcsWidget widget) {
        lock.lock();
        try {
            widgets.add(widget);
        } finally {
            lock.unlock();
        }
    }

    public void removeWidget(BcsWidget widget) {
        lock.lock();
        try {
            widgets.remove(widget);
        } finally {
            lock.unlock();
        }
    }

    public List<BcsWidget> getWidgets() {
        lock.lock();
        try {
            return new ArrayList<>(widgets);
        } finally {
            lock.unlock();
        }
    }
}
