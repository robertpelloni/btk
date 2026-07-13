package graphics

import (
	"sync"
	core "github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

type BcsPainter struct {
	*core.BcsObject
	canvas *BcsCanvas
	active bool
	mu     sync.Mutex
}

func NewBcsPainter(parent *core.BcsObject) *BcsPainter {
	return &BcsPainter{
		BcsObject: core.NewBcsObject(parent),
		active:    false,
	}
}

func (p *BcsPainter) Begin(canvas *BcsCanvas) bool {
	p.mu.Lock()
	defer p.mu.Unlock()
	if p.active {
		return false
	}
	p.canvas = canvas
	p.active = true
	return true
}

func (p *BcsPainter) End() {
	p.mu.Lock()
	defer p.mu.Unlock()
	p.active = false
	p.canvas = nil
}
