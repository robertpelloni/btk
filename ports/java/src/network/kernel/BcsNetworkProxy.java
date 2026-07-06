package network.kernel;

import core.kernel.BcsObject;
import java.util.concurrent.locks.ReentrantLock;

public class BcsNetworkProxy extends BcsObject {
    public enum ProxyType {
        DefaultProxy,
        Socks5Proxy,
        NoProxy,
        HttpProxy,
        HttpCachingProxy,
        FtpCachingProxy
    }

    private ProxyType proxyType = ProxyType.DefaultProxy;
    private String host = "";
    private int port = 0;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsNetworkProxy() {
        super(null);
    }

    public void setType(ProxyType type) {
        lock.lock();
        try {
            this.proxyType = type;
        } finally {
            lock.unlock();
        }
    }

    public void setHostName(String host) {
        lock.lock();
        try {
            this.host = host;
        } finally {
            lock.unlock();
        }
    }

    public void setPort(int port) {
        lock.lock();
        try {
            this.port = port;
        } finally {
            lock.unlock();
        }
    }
}
