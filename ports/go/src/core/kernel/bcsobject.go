package kernel

import (
	"sync"
	"fmt"
)

// BcsObject forms the base for all framework components in the Go port, mapping to C++ QObject
type BcsObject struct {
	mu           sync.Mutex
	parent       *BcsObject
	children     []*BcsObject
	objectName   string
	dispatcher   *BcsEventDispatcher
	isDestroyed  bool
}

func NewBcsObject(parent *BcsObject) *BcsObject {
	obj := &BcsObject{
		parent:     parent,
		children:   make([]*BcsObject, 0),
		dispatcher: nil, // Would default to thread-local dispatcher in full implementation
	}

	if parent != nil {
		parent.AddChild(obj)
	}
	return obj
}

func (o *BcsObject) SetObjectName(name string) {
	o.mu.Lock()
	defer o.mu.Unlock()
	o.objectName = name
}

func (o *BcsObject) ObjectName() string {
	o.mu.Lock()
	defer o.mu.Unlock()
	return o.objectName
}

func (o *BcsObject) Parent() *BcsObject {
	o.mu.Lock()
	defer o.mu.Unlock()
	return o.parent
}

func (o *BcsObject) AddChild(child *BcsObject) {
	o.mu.Lock()
	defer o.mu.Unlock()

	// Check if already a child
	for _, c := range o.children {
		if c == child {
			return
		}
	}

	o.children = append(o.children, child)
	child.SetParent(o)
}

func (o *BcsObject) RemoveChild(child *BcsObject) {
	o.mu.Lock()
	defer o.mu.Unlock()

	for i, c := range o.children {
		if c == child {
			// Remove element
			o.children[i] = o.children[len(o.children)-1]
			o.children = o.children[:len(o.children)-1]
			child.SetParent(nil)
			return
		}
	}
}

func (o *BcsObject) SetParent(parent *BcsObject) {
	o.mu.Lock()
	oldParent := o.parent
	o.parent = parent
	o.mu.Unlock()

	if oldParent != nil && oldParent != parent {
		oldParent.RemoveChild(o)
	}
	if parent != nil {
		parent.AddChild(o)
	}
}

// Destroy cleans up the object tree recursively (simulating C++ destructor)
func (o *BcsObject) Destroy() {
	o.mu.Lock()
	if o.isDestroyed {
		o.mu.Unlock()
		return
	}
	o.isDestroyed = true

	// Copy children to avoid deadlock during destruction
	childrenCopy := make([]*BcsObject, len(o.children))
	copy(childrenCopy, o.children)
	o.mu.Unlock()

	for _, child := range childrenCopy {
		child.Destroy()
	}

	o.SetParent(nil)
	o.children = nil
}

// Event receives events from the dispatcher
func (o *BcsObject) Event(e BcsEvent) bool {
	// Virtual function mapping. Subclasses handle specific events.
	if e.Type() == Timer {
		o.TimerEvent(e.(*BcsTimerEvent))
		return true
	} else if e.Type() == ChildAdded || e.Type() == ChildRemoved {
		o.ChildEvent(e.(*BcsChildEvent))
		return true
	}
	return false
}

// Event filters
func (o *BcsObject) TimerEvent(e *BcsTimerEvent) {}
func (o *BcsObject) ChildEvent(e *BcsChildEvent) {}
