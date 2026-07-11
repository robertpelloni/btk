package kernel

import (
	"testing"
	core "github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

func TestBcsTcpSocket_ObjectLifecycle(t *testing.T) {
	parent := core.NewBcsObject(nil)
	socket := NewBcsTcpSocket(parent)

	if socket.Parent() != parent {
		t.Fatalf("Expected parent to be set correctly")
	}
}
