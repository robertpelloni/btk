package kernel

import (
	"sync"
	"github.com/robertpelloni/btk-go/src/core/kernel"
)

// BcsWidget extends the core BcsWidget mapped in core to include GUI event handlers
type BcsWidget struct {
	*kernel.BcsWidget
	focus bool
	mu    sync.RWMutex
}

func NewBcsWidget(parent *kernel.BcsWidget) *BcsWidget {
	return &BcsWidget{
		BcsWidget: kernel.NewBcsWidget(parent),
		focus:     false,
	}
}

func (w *BcsWidget) HasFocus() bool {
	w.mu.RLock()
	defer w.mu.RUnlock()
	return w.focus
}

func (w *BcsWidget) SetFocus(f bool) {
	w.mu.Lock()
	defer w.mu.Unlock()
	w.focus = f
}

// Event overrides the base object event router to handle GUI events
func (w *BcsWidget) Event(e kernel.BcsEvent) bool {
	switch e.Type() {
	case kernel.MouseButtonPress:
		w.MousePressEvent(e)
		return true
	case kernel.MouseButtonRelease:
		w.MouseReleaseEvent(e)
		return true
	case kernel.KeyPress:
		w.KeyPressEvent(e)
		return true
	case kernel.KeyRelease:
		w.KeyReleaseEvent(e)
		return true
	case kernel.Paint:
		w.PaintEvent(e)
		return true
	}
	return w.BcsWidget.Event(e)
}

func (w *BcsWidget) MousePressEvent(e kernel.BcsEvent) {}
func (w *BcsWidget) MouseReleaseEvent(e kernel.BcsEvent) {}
func (w *BcsWidget) KeyPressEvent(e kernel.BcsEvent) {}
func (w *BcsWidget) KeyReleaseEvent(e kernel.BcsEvent) {}
func (w *BcsWidget) PaintEvent(e kernel.BcsEvent) {}
