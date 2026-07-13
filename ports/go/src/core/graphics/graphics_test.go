package graphics

import (
	"testing"
	core "github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

func TestGraphicsLifecycle(t *testing.T) {
	parent := core.NewBcsObject(nil)
	canvas := NewBcsCanvas(parent, 800, 600)
	painter := NewBcsPainter(parent)

	if canvas.Parent() != parent {
		t.Fatalf("Canvas parent not set correctly")
	}

	w, h := canvas.Size()
	if w != 800 || h != 600 {
		t.Fatalf("Canvas size incorrect")
	}

	if !painter.Begin(canvas) {
		t.Fatalf("Painter begin failed")
	}

	if painter.Begin(canvas) {
		t.Fatalf("Painter double begin should fail")
	}

	painter.End()
}
