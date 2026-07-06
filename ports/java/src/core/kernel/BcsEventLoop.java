package core.kernel;

import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

public class BcsEventLoop extends BcsObject {
    private final BcsEventDispatcher dispatcher;

    public BcsEventDispatcher getDispatcher() {
        return dispatcher;
    }

    private final AtomicBoolean isRunning = new AtomicBoolean(false);
    private final AtomicInteger quitCode = new AtomicInteger(0);

    public BcsEventLoop() {
        this(new BcsEventDispatcher(), null);
    }

    public BcsEventLoop(BcsEventDispatcher dispatcher, BcsObject parent) {
        super(parent);
        this.dispatcher = dispatcher != null ? dispatcher : new BcsEventDispatcher();
    }

    public int exec() {
        if (!isRunning.compareAndSet(false, true)) {
            return -1;
        }

        dispatcher.exec();

        isRunning.set(false);
        return quitCode.get();
    }

    public void exit(int returnCode) {
        quitCode.set(returnCode);
        dispatcher.stop();
    }

    public void quit() {
        exit(0);
    }

    public boolean isRunning() {
        return isRunning.get();
    }
}
