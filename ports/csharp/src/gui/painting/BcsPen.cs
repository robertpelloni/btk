using System;
using Bcs.Core.Kernel;

namespace Bcs.Gui.Painting
{
    public enum PenStyle
    {
        NoPen = 0,
        SolidLine,
        DashLine,
        DotLine,
        DashDotLine,
        DashDotDotLine,
        CustomDashLine
    }

    public enum PenCapStyle
    {
        FlatCap = 0,
        SquareCap,
        RoundCap
    }

    public enum PenJoinStyle
    {
        MiterJoin = 0,
        BevelJoin,
        RoundJoin,
        SvgMiterJoin
    }

    public class BcsPen : BcsObject
    {
        private PenStyle _style = PenStyle.SolidLine;
        private string _color = "#000000";
        private double _width = 1.0;
        private PenCapStyle _capStyle = PenCapStyle.SquareCap;
        private PenJoinStyle _joinStyle = PenJoinStyle.BevelJoin;
        private readonly object _lock = new object();

        public BcsPen() : base(null) {}

        public PenStyle Style
        {
            get { lock (_lock) { return _style; } }
            set { lock (_lock) { _style = value; } }
        }

        public string Color
        {
            get { lock (_lock) { return _color; } }
            set { lock (_lock) { _color = value; } }
        }

        public double Width
        {
            get { lock (_lock) { return _width; } }
            set { lock (_lock) { _width = value; } }
        }

        public PenCapStyle CapStyle
        {
            get { lock (_lock) { return _capStyle; } }
            set { lock (_lock) { _capStyle = value; } }
        }

        public PenJoinStyle JoinStyle
        {
            get { lock (_lock) { return _joinStyle; } }
            set { lock (_lock) { _joinStyle = value; } }
        }
    }
}
