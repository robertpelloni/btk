package kernel

import "sync"

// BcsWidget maps to QWidget base class
type BcsWidget struct {
	*BcsObject
	visible   bool
	enabled   bool
	width     int
	height    int
	x         int
	y         int
	mu        sync.RWMutex
}

func NewBcsWidget(parent *BcsWidget) *BcsWidget {
	var parentObj *BcsObject
	if parent != nil {
		parentObj = parent.BcsObject
	}
	w := &BcsWidget{
		BcsObject: NewBcsObject(parentObj),
		visible:   false,
		enabled:   true,
		width:     0,
		height:    0,
		x:         0,
		y:         0,
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
	// Fire resize event
}

func (w *BcsWidget) Move(x, y int) {
	w.mu.Lock()
	defer w.mu.Unlock()
	w.x = x
	w.y = y
	// Fire move event
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
