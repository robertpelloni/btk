package widgets

import (
	"sync"
	core "github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
	gui "github.com/robertpelloni/btk-go/ports/go/src/gui/kernel"
)

type BcsInput struct {
	*gui.BcsWidget
	text string
	mu   sync.RWMutex
}

func NewBcsInput(parent *gui.BcsWidget) *BcsInput {
	var p *core.BcsObject
	if parent != nil {
		p = parent.BcsObject
	}
	i := &BcsInput{
		BcsWidget: gui.NewBcsWidget(p),
		text:      "",
	}
	i.Impl = i
	return i
}

func (i *BcsInput) SetText(t string) {
	i.mu.Lock()
	defer i.mu.Unlock()
	i.text = t
}

func (i *BcsInput) Text() string {
	i.mu.RLock()
	defer i.mu.RUnlock()
	return i.text
}
