package core.kernel;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.locks.ReentrantLock;

public class BcsTextStream {
    private final ByteArrayOutputStream buffer = new ByteArrayOutputStream();
    private final PrintStream writer = new PrintStream(buffer, true, StandardCharsets.UTF_8);
    private final ReentrantLock lock = new ReentrantLock();

    public void writeString(String text) {
        lock.lock();
        try {
            writer.print(text);
        } finally {
            lock.unlock();
        }
    }

    public String readAll() {
        lock.lock();
        try {
            return buffer.toString(StandardCharsets.UTF_8);
        } finally {
            lock.unlock();
        }
    }

    public void flush() {
        lock.lock();
        try {
            writer.flush();
        } finally {
            lock.unlock();
        }
    }
}
