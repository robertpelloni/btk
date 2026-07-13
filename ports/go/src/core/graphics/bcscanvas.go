package graphics

import (
	"sync"
	core "github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

type BcsCanvas struct {
	*core.BcsObject
	width  int
	height int
	mu     sync.RWMutex
}

func NewBcsCanvas(parent *core.BcsObject, width, height int) *BcsCanvas {
	return &BcsCanvas{
		BcsObject: core.NewBcsObject(parent),
		width:     width,
		height:    height,
	}
}

func (c *BcsCanvas) Size() (int, int) {
	c.mu.RLock()
	defer c.mu.RUnlock()
	return c.width, c.height
}
