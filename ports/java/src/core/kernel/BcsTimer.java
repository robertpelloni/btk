package core.kernel;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReentrantLock;

public class BcsTimer extends BcsObject {
    private static final AtomicInteger timerIdCounter = new AtomicInteger(1);

    private int intervalMs = 0;
    private boolean singleShot = false;
    private final AtomicBoolean isActive = new AtomicBoolean(false);
    private final int timerId;
    private ScheduledExecutorService executor;
    private ScheduledFuture<?> scheduledTask;
    private Runnable timeoutCallback;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsTimer() {
        this(null);
    }

    public BcsTimer(BcsObject parent) {
        super(parent);
        this.timerId = timerIdCounter.getAndIncrement();
    }

    public int getTimerId() {
        return timerId;
    }

    public void setInterval(int msec) {
        lock.lock();
        try {
            this.intervalMs = msec;
        } finally {
            lock.unlock();
        }
    }

    public void setSingleShot(boolean singleShot) {
        lock.lock();
        try {
            this.singleShot = singleShot;
        } finally {
            lock.unlock();
        }
    }

    public boolean isActive() {
        return isActive.get();
    }

    public void connectTimeout(Runnable r) {
        lock.lock();
        try {
            this.timeoutCallback = r;
        } finally {
            lock.unlock();
        }
    }

    public void start(int msec) {
        if (msec > 0) {
            setInterval(msec);
        }
        start();
    }

    public void start() {
        stop();

        lock.lock();
        try {
            isActive.set(true);
            executor = Executors.newSingleThreadScheduledExecutor();

            Runnable task = () -> {
                if (!isActive.get()) return;

                if (timeoutCallback != null) {
                    timeoutCallback.run();
                }
                dispatchTimerEvent();

                if (singleShot) {
                    isActive.set(false);
                }
            };

            if (singleShot) {
                scheduledTask = executor.schedule(task, intervalMs, TimeUnit.MILLISECONDS);
            } else {
                scheduledTask = executor.scheduleAtFixedRate(task, intervalMs, intervalMs, TimeUnit.MILLISECONDS);
            }
        } finally {
            lock.unlock();
        }
    }

    public void stop() {
        if (!isActive.compareAndSet(true, false)) {
            return;
        }

        lock.lock();
        try {
            if (scheduledTask != null) {
                scheduledTask.cancel(false);
            }
            if (executor != null) {
                executor.shutdown();
            }
        } finally {
            lock.unlock();
        }
    }

    private void dispatchTimerEvent() {
        BcsTimerEvent e = new BcsTimerEvent(timerId);
        BcsKernel.getInstance().getEventLoop().getDispatcher().postEvent(e);
    }
}
