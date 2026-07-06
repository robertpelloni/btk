package widgets

import (
	"sync"
	"github.com/robertpelloni/btk-go/ports/go/src/gui/kernel"
)

// BcsWindow
type BcsWindow struct {
	*kernel.BcsWidget
	title string
	mu    sync.RWMutex
}

func NewBcsWindow() *BcsWindow {
	w := &BcsWindow{
		BcsWidget: kernel.NewBcsWidget(nil), // Windows have no parent
		title:     "",
	}
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
	*kernel.BcsWidget
	text     string
	onClick  func()
	mu       sync.RWMutex
}

func NewBcsButton(parent *kernel.BcsWidget) *BcsButton {
	return &BcsButton{
		BcsWidget: kernel.NewBcsWidget(parent.BcsObject),
		text:      "",
	}
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

// BcsLabel
type BcsLabel struct {
	*kernel.BcsWidget
	text string
	mu   sync.RWMutex
}

func NewBcsLabel(parent *kernel.BcsWidget) *BcsLabel {
	return &BcsLabel{
		BcsWidget: kernel.NewBcsWidget(parent.BcsObject),
		text:      "",
	}
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
