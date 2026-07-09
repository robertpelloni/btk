package gui.painting;

import core.kernel.BcsObject;
import java.util.concurrent.locks.ReentrantLock;

public class BcsBrush extends BcsObject {
    public enum BrushStyle {
        NoBrush,
        SolidPattern,
        Dense1Pattern,
        Dense2Pattern,
        Dense3Pattern,
        Dense4Pattern,
        Dense5Pattern,
        Dense6Pattern,
        Dense7Pattern,
        HorPattern,
        VerPattern,
        CrossPattern,
        BDiagPattern,
        FDiagPattern,
        DiagCrossPattern,
        LinearGradientPattern,
        RadialGradientPattern,
        ConicalGradientPattern,
        TexturePattern
    }

    private BrushStyle style = BrushStyle.NoBrush;
    private String color = "#000000";
    private final ReentrantLock lock = new ReentrantLock();

    public BcsBrush() {
        super(null);
    }

    public BcsBrush(String color, BrushStyle style) {
        super(null);
        this.color = color;
        this.style = style;
    }

    public BrushStyle getStyle() {
        lock.lock();
        try {
            return style;
        } finally {
            lock.unlock();
        }
    }

    public void setStyle(BrushStyle style) {
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
}
