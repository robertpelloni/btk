package kernel

import (
	"sync"
	"github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

// BcsWidget extends the core BcsWidget mapped in core to include GUI event handlers
type BcsWidget struct {
	*kernel.BcsObject
	visible   bool
	enabled   bool
	width     int
	height    int
	x         int
	y         int
	focus     bool
	mu        sync.RWMutex
}

func NewBcsWidget(parent *kernel.BcsObject) *BcsWidget {
	w := &BcsWidget{
		BcsObject: kernel.NewBcsObject(parent),
		visible:   false,
		enabled:   true,
		width:     0,
		height:    0,
		x:         0,
		y:         0,
		focus:     false,
	}
	return w
}

func (w *BcsWidget) Show() {
	w.mu.Lock()
	defer w.mu.Unlock()
	w.visible = true
}

func (w *BcsWidget) Hide() {
	w.mu.Lock()
	defer w.mu.Unlock()
	w.visible = false
}

func (w *BcsWidget) IsVisible() bool {
	w.mu.RLock()
	defer w.mu.RUnlock()
	return w.visible
}

func (w *BcsWidget) Resize(width, height int) {
	w.mu.Lock()
	defer w.mu.Unlock()
	w.width = width
	w.height = height
}

func (w *BcsWidget) Move(x, y int) {
	w.mu.Lock()
	defer w.mu.Unlock()
	w.x = x
	w.y = y
}

func (w *BcsWidget) SetEnabled(enabled bool) {
	w.mu.Lock()
	defer w.mu.Unlock()
	w.enabled = enabled
}

func (w *BcsWidget) IsEnabled() bool {
	w.mu.RLock()
	defer w.mu.RUnlock()
	return w.enabled
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
	return w.BcsObject.Event(e)
}

func (w *BcsWidget) MousePressEvent(e kernel.BcsEvent) {}
func (w *BcsWidget) MouseReleaseEvent(e kernel.BcsEvent) {}
func (w *BcsWidget) KeyPressEvent(e kernel.BcsEvent) {}
func (w *BcsWidget) KeyReleaseEvent(e kernel.BcsEvent) {}
func (w *BcsWidget) PaintEvent(e kernel.BcsEvent) {}
