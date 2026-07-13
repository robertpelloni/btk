package core.kernel;

import java.io.IOException;
import java.util.concurrent.locks.ReentrantLock;

public class BcsProcess extends BcsObject {
    private Process process;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsProcess(BcsObject parent) {
        super(parent);
    }

    public void start(String command, String... args) throws IOException {
        lock.lock();
        try {
            String[] cmdArray = new String[args.length + 1];
            cmdArray[0] = command;
            System.arraycopy(args, 0, cmdArray, 1, args.length);

            ProcessBuilder pb = new ProcessBuilder(cmdArray);
            process = pb.start();
        } finally {
            lock.unlock();
        }
    }

    public void terminate() {
        lock.lock();
        try {
            if (process != null && process.isAlive()) {
                process.destroy();
            }
        } finally {
            lock.unlock();
        }
    }

    public int waitFor() throws InterruptedException {
        Process p;
        lock.lock();
        try {
            p = process;
        } finally {
            lock.unlock();
        }

        if (p != null) {
            return p.waitFor();
        }
        return -1;
    }
}
