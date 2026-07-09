package gui.painting;

import core.kernel.BcsObject;
import java.util.concurrent.locks.ReentrantLock;

public class BcsPen extends BcsObject {
    public enum PenStyle {
        NoPen,
        SolidLine,
        DashLine,
        DotLine,
        DashDotLine,
        DashDotDotLine,
        CustomDashLine
    }

    public enum PenCapStyle {
        FlatCap,
        SquareCap,
        RoundCap
    }

    public enum PenJoinStyle {
        MiterJoin,
        BevelJoin,
        RoundJoin,
        SvgMiterJoin
    }

    private PenStyle style = PenStyle.SolidLine;
    private String color = "#000000";
    private double width = 1.0;
    private PenCapStyle capStyle = PenCapStyle.SquareCap;
    private PenJoinStyle joinStyle = PenJoinStyle.BevelJoin;
    private final ReentrantLock lock = new ReentrantLock();

    public BcsPen() {
        super(null);
    }

    public PenStyle getStyle() {
        lock.lock();
        try {
            return style;
        } finally {
            lock.unlock();
        }
    }

    public void setStyle(PenStyle style) {
        lock.lock();
        try {
            this.style = style;
        } finally {
            lock.unlock();
        }
    }

    public String getColor() {
        lock.lock();
        try {
            return color;
        } finally {
            lock.unlock();
        }
    }

    public void setColor(String color) {
        lock.lock();
        try {
            this.color = color;
        } finally {
            lock.unlock();
        }
    }

    public double getWidth() {
        lock.lock();
        try {
            return width;
        } finally {
            lock.unlock();
        }
    }

    public void setWidth(double width) {
        lock.lock();
        try {
            this.width = width;
        } finally {
            lock.unlock();
        }
    }

    public PenCapStyle getCapStyle() {
        lock.lock();
        try {
            return capStyle;
        } finally {
            lock.unlock();
        }
    }

    public void setCapStyle(PenCapStyle capStyle) {
        lock.lock();
        try {
            this.capStyle = capStyle;
        } finally {
            lock.unlock();
        }
    }

    public PenJoinStyle getJoinStyle() {
        lock.lock();
        try {
            return joinStyle;
        } finally {
            lock.unlock();
        }
    }

    public void setJoinStyle(PenJoinStyle joinStyle) {
        lock.lock();
        try {
            this.joinStyle = joinStyle;
        } finally {
            lock.unlock();
        }
    }
}
