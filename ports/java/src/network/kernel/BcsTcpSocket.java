package network.kernel;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.concurrent.locks.ReentrantLock;
import core.kernel.BcsObject;

public class BcsTcpSocket extends BcsObject {
    private Socket socket;
    private InputStream in;
    private OutputStream out;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsTcpSocket(BcsObject parent) {
        super(parent);
    }

    public void connectToHost(String address, int port) throws IOException {
        lock.lock();
        try {
            if (socket != null && !socket.isClosed()) {
                socket.close();
            }
            socket = new Socket(address, port);
            in = socket.getInputStream();
            out = socket.getOutputStream();
        } finally {
            lock.unlock();
        }
    }

    public int write(byte[] data) throws IOException {
        lock.lock();
        try {
            if (out == null) throw new IOException("Socket not connected");
            out.write(data);
            return data.length;
        } finally {
            lock.unlock();
        }
    }

    public int read(byte[] data) throws IOException {
        lock.lock();
        try {
            if (in == null) throw new IOException("Socket not connected");
            return in.read(data);
        } finally {
            lock.unlock();
        }
    }

    public void close() throws IOException {
        lock.lock();
        try {
            if (socket != null && !socket.isClosed()) {
                socket.close();
            }
            in = null;
            out = null;
        } finally {
            lock.unlock();
        }
    }
}
