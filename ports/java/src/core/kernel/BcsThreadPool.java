package core.kernel;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

public class BcsThreadPool extends BcsObject {
    private final ExecutorService executor;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsThreadPool(BcsObject parent, int numWorkers) {
        super(parent);
        this.executor = Executors.newFixedThreadPool(numWorkers);
    }

    public void submit(Runnable task) {
        lock.lock();
        try {
            if (!executor.isShutdown()) {
                executor.submit(task);
            }
        } finally {
            lock.unlock();
        }
    }

    public void shutdown() {
        lock.lock();
        try {
            if (!executor.isShutdown()) {
                executor.shutdown();
            }
        } finally {
            lock.unlock();
        }

        // Wait outside the lock so we don't stall new submit calls
        try {
            if (!executor.awaitTermination(60, TimeUnit.SECONDS)) {
                executor.shutdownNow();
            }
        } catch (InterruptedException ex) {
            executor.shutdownNow();
            Thread.currentThread().interrupt();
        }
    }
}
