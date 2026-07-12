package widgets

import (
	"sync"
	core "github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
	gui "github.com/robertpelloni/btk-go/ports/go/src/gui/kernel"
)

type BcsLayout struct {
	*core.BcsObject
	widgets []*gui.BcsWidget
	mu      sync.Mutex
}

func NewBcsLayout(parent *gui.BcsWidget) *BcsLayout {
	l := &BcsLayout{
		BcsObject: core.NewBcsObject(nil),
		widgets:   make([]*gui.BcsWidget, 0),
	}
	if parent != nil {
		l.SetParent(parent.BcsObject)
	}
	return l
}

func (l *BcsLayout) AddWidget(w *gui.BcsWidget) {
	l.mu.Lock()
	defer l.mu.Unlock()
	l.widgets = append(l.widgets, w)
}

func (l *BcsLayout) RemoveWidget(w *gui.BcsWidget) {
	l.mu.Lock()
	defer l.mu.Unlock()
	for i, widget := range l.widgets {
		if widget == w {
			l.widgets = append(l.widgets[:i], l.widgets[i+1:]...)
			break
		}
	}
}

func (l *BcsLayout) Widgets() []*gui.BcsWidget {
	l.mu.Lock()
	defer l.mu.Unlock()
	widgetsCopy := make([]*gui.BcsWidget, len(l.widgets))
	copy(widgetsCopy, l.widgets)
	return widgetsCopy
}
