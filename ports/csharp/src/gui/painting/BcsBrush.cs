using System;
using Bcs.Core.Kernel;

namespace Bcs.Gui.Painting
{
    public enum BrushStyle
    {
        NoBrush = 0,
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

    public class BcsBrush : BcsObject
    {
        private BrushStyle _style = BrushStyle.NoBrush;
        private string _color = "#000000";
        private readonly object _lock = new object();

        public BcsBrush() : base(null) {}

        public BcsBrush(string color, BrushStyle style) : base(null)
        {
            _color = color;
            _style = style;
        }

        public BrushStyle Style
        {
            get { lock (_lock) { return _style; } }
            set { lock (_lock) { _style = value; } }
        }

        public string Color
        {
            get { lock (_lock) { return _color; } }
            set { lock (_lock) { _color = value; } }
        }
    }
}
