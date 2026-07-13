using System;
using System.Diagnostics;
using Bcs.Core.Kernel;

namespace Bcs.Core.Graphics.Tests
{
    public class BcsGraphicsTests
    {
        public static void TestGraphicsLifecycle()
        {
            var parent = new BcsObject();
            var canvas = new BcsCanvas(parent, 800, 600);
            var painter = new BcsPainter(parent);

            Debug.Assert(canvas.Parent() == parent);
            var (w, h) = canvas.Size();
            Debug.Assert(w == 800 && h == 600);

            Debug.Assert(painter.Begin(canvas));
            Debug.Assert(!painter.Begin(canvas));

            painter.End();
        }
    }
}
