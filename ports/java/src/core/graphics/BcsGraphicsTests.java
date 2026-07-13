package core.graphics;

import core.kernel.BcsObject;

public class BcsGraphicsTests {
    public static void testGraphicsLifecycle() {
        BcsObject parent = new BcsObject(null);
        BcsCanvas canvas = new BcsCanvas(parent, 800, 600);
        BcsPainter painter = new BcsPainter(parent);

        if (canvas.getParent() != parent) {
            throw new AssertionError("Parent mismatch");
        }

        int[] size = canvas.getSize();
        if (size[0] != 800 || size[1] != 600) {
            throw new AssertionError("Size mismatch");
        }

        if (!painter.begin(canvas)) {
            throw new AssertionError("Begin failed");
        }

        if (painter.begin(canvas)) {
            throw new AssertionError("Double begin should fail");
        }

        painter.end();
    }
}
