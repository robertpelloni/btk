package kernel

import (
	"testing"
	core "github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

func TestBcsWidgetEventRouting(t *testing.T) {
	window := core.NewBcsObject(nil)
	widget := NewBcsWidget(window)
	arbitrator := GetInputArbitrator()

	// Create a mock mouse press event
	event := core.NewEvent(core.MouseButtonPress)

	// Route event to widget via arbitrator
	handled := arbitrator.RouteEvent(event, widget)

	if !handled {
		t.Errorf("Expected RouteEvent to handle MouseButtonPress event on BcsWidget, got false")
	}
}
