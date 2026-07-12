package widgets

import (
	"sync"
	"github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
	gui "github.com/robertpelloni/btk-go/ports/go/src/gui/kernel"
)

// BcsWindow
type BcsWindow struct {
	*gui.BcsWidget
	title string
	mu    sync.RWMutex
}

func NewBcsWindow() *BcsWindow {
	w := &BcsWindow{
		BcsWidget: gui.NewBcsWidget(nil), // Windows have no parent
		title:     "",
	}
	w.Impl = w
	return w
}

func (w *BcsWindow) SetTitle(t string) {
	w.mu.Lock()
	defer w.mu.Unlock()
	w.title = t
}

func (w *BcsWindow) Title() string {
	w.mu.RLock()
	defer w.mu.RUnlock()
	return w.title
}

// BcsButton
type BcsButton struct {
	*gui.BcsWidget
	text     string
	onClick  func()
	mu       sync.RWMutex
}

func NewBcsButton(parent *gui.BcsWidget) *BcsButton {
	var p *kernel.BcsObject
	if parent != nil {
		p = parent.BcsObject
	}
	b := &BcsButton{
		BcsWidget: gui.NewBcsWidget(p),
		text:      "",
	}
	b.Impl = b
	return b
}

func (b *BcsButton) SetText(t string) {
	b.mu.Lock()
	defer b.mu.Unlock()
	b.text = t
}

func (b *BcsButton) Text() string {
	b.mu.RLock()
	defer b.mu.RUnlock()
	return b.text
}

func (b *BcsButton) ConnectClicked(f func()) {
	b.mu.Lock()
	defer b.mu.Unlock()
	b.onClick = f
}

func (b *BcsButton) MouseReleaseEvent(e kernel.BcsEvent) {
	b.mu.RLock()
	defer b.mu.RUnlock()
	if b.onClick != nil {
		b.onClick()
	}
}

// BcsLabel
type BcsLabel struct {
	*gui.BcsWidget
	text string
	mu   sync.RWMutex
}

func NewBcsLabel(parent *gui.BcsWidget) *BcsLabel {
	var p *kernel.BcsObject
	if parent != nil {
		p = parent.BcsObject
	}
	l := &BcsLabel{
		BcsWidget: gui.NewBcsWidget(p),
		text:      "",
	}
	l.Impl = l
	return l
}

func (l *BcsLabel) SetText(t string) {
	l.mu.Lock()
	defer l.mu.Unlock()
	l.text = t
}

func (l *BcsLabel) Text() string {
	l.mu.RLock()
	defer l.mu.RUnlock()
	return l.text
}
