package kernel

import (
	"testing"
)

func TestBcsObjectLifecycle(t *testing.T) {
	parent := NewBcsObject(nil)
	child1 := NewBcsObject(parent)
	child2 := NewBcsObject(parent)

	if len(parent.children) != 2 {
		t.Errorf("Expected parent to have 2 children, got %d", len(parent.children))
	}

	if child1.Parent() != parent {
		t.Errorf("Expected child1 to have parent %v, got %v", parent, child1.Parent())
	}

	child2.Destroy()

	if len(parent.children) != 1 {
		t.Errorf("Expected parent to have 1 child after child2 destroyed, got %d", len(parent.children))
	}
}

func TestBcsEventDispatcher(t *testing.T) {
	dispatcher := NewBcsEventDispatcher()
	event := NewEvent(Timer)

	dispatcher.PostEvent(event)

	if len(dispatcher.eventQueue) != 1 {
		t.Errorf("Expected 1 event in queue, got %d", len(dispatcher.eventQueue))
	}
}

func TestBcsKernelStartup(t *testing.T) {
    k := GetKernel()

    if k.isStarted {
        t.Errorf("Expected kernel to not be started initially")
    }
}
