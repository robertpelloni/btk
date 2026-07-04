package core.kernel;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class BcsEventDispatcher {
    private final BlockingQueue<BcsEvent> eventQueue = new LinkedBlockingQueue<>();
    private final AtomicBoolean running = new AtomicBoolean(false);

    public void postEvent(BcsEvent event) {
        eventQueue.add(event);
    }

    public void processEvents() {
        BcsEvent event;
        while ((event = eventQueue.poll()) != null) {
            dispatchEvent(event);
        }
    }

    public void exec() {
        running.set(true);
        try {
            while (running.get()) {
                BcsEvent event = eventQueue.take();
                dispatchEvent(event);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    public void stop() {
        running.set(false);
        // Post a dummy event to wake up the blocked take() call
        postEvent(new BcsEvent(BcsEvent.EventType.None) {});
    }

    private void dispatchEvent(BcsEvent event) {
        // Route event to target
    }
}
