package core.kernel;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicBoolean;

public class BcsKernel extends BcsObject {

    public interface BcsModule {
        void initialize() throws Exception;
        void start() throws Exception;
        void shutdown();
        String getName();
    }

    private static volatile BcsKernel instance;
    private final List<BcsModule> modules = new CopyOnWriteArrayList<>();
    private final BcsEventLoop eventLoop;

    public BcsEventLoop getEventLoop() {
        return eventLoop;
    }


    private final AtomicBoolean isStarted = new AtomicBoolean(false);
    private final AtomicBoolean isShuttingDown = new AtomicBoolean(false);

    private BcsKernel() {
        super(null);
        this.eventLoop = new BcsEventLoop(new BcsEventDispatcher(), this);
    }

    public static BcsKernel getInstance() {
        if (instance == null) {
            synchronized (BcsKernel.class) {
                if (instance == null) {
                    instance = new BcsKernel();
                }
            }
        }
        return instance;
    }

    public void registerModule(BcsModule module) {
        if (!isStarted.get()) {
            modules.add(module);
        }
    }

    public void startup() throws Exception {
        if (!isStarted.compareAndSet(false, true)) {
            return;
        }

        // Phase 1: Initialize
        for (BcsModule mod : modules) {
            System.out.println("BcsKernel: Initializing module " + mod.getName());
            mod.initialize();
        }

        // Phase 2: Start
        for (BcsModule mod : modules) {
            System.out.println("BcsKernel: Starting module " + mod.getName());
            mod.start();
        }
    }

    public int exec() {
        try {
            startup();
        } catch (Exception ex) {
            System.out.println("BcsKernel Startup Error: " + ex.getMessage());
            return -1;
        }

        System.out.println("BcsKernel: Entering main event loop...");
        int code = eventLoop.exec();

        shutdown();
        return code;
    }

    public void shutdown() {
        if (!isShuttingDown.compareAndSet(false, true)) {
            return;
        }

        System.out.println("BcsKernel: Shutting down...");

        List<BcsModule> modulesCopy = new ArrayList<>(modules);
        for (int i = modulesCopy.size() - 1; i >= 0; i--) {
            BcsModule mod = modulesCopy.get(i);
            System.out.println("BcsKernel: Shutting down module " + mod.getName());
            try {
                mod.shutdown();
            } catch (Exception e) {
                // Ignore cleanup errors
            }
        }

        close();
    }
}
