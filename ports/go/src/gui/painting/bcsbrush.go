package painting

import (
	"sync"
	"github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

type BrushStyle int
const (
	NoBrush BrushStyle = iota
	SolidPattern
	Dense1Pattern
	Dense2Pattern
	Dense3Pattern
	Dense4Pattern
	Dense5Pattern
	Dense6Pattern
	Dense7Pattern
	HorPattern
	VerPattern
	CrossPattern
	BDiagPattern
	FDiagPattern
	DiagCrossPattern
	LinearGradientPattern
	RadialGradientPattern
	ConicalGradientPattern
	TexturePattern
)

// BcsBrush maps to QBrush
type BcsBrush struct {
	*kernel.BcsObject
	style BrushStyle
	color string // Using string for color hex code simplistically
	mu    sync.RWMutex
}

func NewBcsBrush() *BcsBrush {
	return &BcsBrush{
		BcsObject: kernel.NewBcsObject(nil),
		style:     NoBrush,
		color:     "#000000",
	}
}

func NewBcsBrushWithColor(color string, style BrushStyle) *BcsBrush {
	return &BcsBrush{
		BcsObject: kernel.NewBcsObject(nil),
		style:     style,
		color:     color,
	}
}

func (b *BcsBrush) SetStyle(s BrushStyle) {
	b.mu.Lock()
	defer b.mu.Unlock()
	b.style = s
}

func (b *BcsBrush) Style() BrushStyle {
	b.mu.RLock()
	defer b.mu.RUnlock()
	return b.style
}

func (b *BcsBrush) SetColor(color string) {
	b.mu.Lock()
	defer b.mu.Unlock()
	b.color = color
}

func (b *BcsBrush) Color() string {
	b.mu.RLock()
	defer b.mu.RUnlock()
	return b.color
}
