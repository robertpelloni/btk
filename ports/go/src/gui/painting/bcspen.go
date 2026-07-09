package painting

import (
	"sync"
	"github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

type PenStyle int
const (
	NoPen PenStyle = iota
	SolidLine
	DashLine
	DotLine
	DashDotLine
	DashDotDotLine
	CustomDashLine
)

type PenCapStyle int
const (
	FlatCap PenCapStyle = iota
	SquareCap
	RoundCap
)

type PenJoinStyle int
const (
	MiterJoin PenJoinStyle = iota
	BevelJoin
	RoundJoin
	SvgMiterJoin
)

// BcsPen maps to QPen
type BcsPen struct {
	*kernel.BcsObject
	style     PenStyle
	color     string
	width     float64
	capStyle  PenCapStyle
	joinStyle PenJoinStyle
	mu        sync.RWMutex
}

func NewBcsPen() *BcsPen {
	return &BcsPen{
		BcsObject: kernel.NewBcsObject(nil),
		style:     SolidLine,
		color:     "#000000",
		width:     1.0,
		capStyle:  SquareCap,
		joinStyle: BevelJoin,
	}
}

func (p *BcsPen) SetStyle(s PenStyle) {
	p.mu.Lock()
	defer p.mu.Unlock()
	p.style = s
}

func (p *BcsPen) Style() PenStyle {
	p.mu.RLock()
	defer p.mu.RUnlock()
	return p.style
}

func (p *BcsPen) SetColor(color string) {
	p.mu.Lock()
	defer p.mu.Unlock()
	p.color = color
}

func (p *BcsPen) Color() string {
	p.mu.RLock()
	defer p.mu.RUnlock()
	return p.color
}

func (p *BcsPen) SetWidth(w float64) {
	p.mu.Lock()
	defer p.mu.Unlock()
	p.width = w
}

func (p *BcsPen) Width() float64 {
	p.mu.RLock()
	defer p.mu.RUnlock()
	return p.width
}

func (p *BcsPen) SetCapStyle(c PenCapStyle) {
	p.mu.Lock()
	defer p.mu.Unlock()
	p.capStyle = c
}

func (p *BcsPen) CapStyle() PenCapStyle {
	p.mu.RLock()
	defer p.mu.RUnlock()
	return p.capStyle
}

func (p *BcsPen) SetJoinStyle(j PenJoinStyle) {
	p.mu.Lock()
	defer p.mu.Unlock()
	p.joinStyle = j
}

func (p *BcsPen) JoinStyle() PenJoinStyle {
	p.mu.RLock()
	defer p.mu.RUnlock()
	return p.joinStyle
}
