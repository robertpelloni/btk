package core.graphics;

import java.util.concurrent.locks.ReentrantLock;
import core.kernel.BcsObject;

public class BcsImage extends BcsObject {
    private final int width;
    private final int height;
    private final byte[] pixels;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsImage(BcsObject parent, int width, int height) {
        super(parent);
        this.width = width;
        this.height = height;
        this.pixels = new byte[width * height * 4]; // RGBA
    }

    public int[] getSize() {
        lock.lock();
        try {
            return new int[]{width, height};
        } finally {
            lock.unlock();
        }
    }

    public void setPixel(int x, int y, byte r, byte g, byte b, byte a) {
        lock.lock();
        try {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                int idx = (y * width + x) * 4;
                pixels[idx] = r;
                pixels[idx + 1] = g;
                pixels[idx + 2] = b;
                pixels[idx + 3] = a;
            }
        } finally {
            lock.unlock();
        }
    }

    public byte[] getPixels() {
        lock.lock();
        try {
            byte[] copy = new byte[pixels.length];
            System.arraycopy(pixels, 0, copy, 0, pixels.length);
            return copy;
        } finally {
            lock.unlock();
        }
    }
}
