package kernel

import (
	"sync"
)

// BcsObject forms the base for all framework components in the Go port, mapping to C++ QObject
type BcsObject struct {
	mu           sync.Mutex
	onDestroy    func()
	parent       *BcsObject
	children     []*BcsObject
	objectName   string
	dispatcher   *BcsEventDispatcher
	isDestroyed  bool
}

func NewBcsObject(parent *BcsObject) *BcsObject {
	obj := &BcsObject{
		parent:     nil, // We set this via SetParent to ensure correct initial linking
		children:   make([]*BcsObject, 0),
		dispatcher: nil,
	}

	if parent != nil {
		obj.SetParent(parent)
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

// addChild is an internal helper that must be called with the parent's lock already held.
func (o *BcsObject) addChild(child *BcsObject) {
	for _, c := range o.children {
		if c == child {
			return
		}
	}
	o.children = append(o.children, child)
}

// removeChild is an internal helper that must be called with the parent's lock already held.
func (o *BcsObject) removeChild(child *BcsObject) {
	for i, c := range o.children {
		if c == child {
			o.children[i] = o.children[len(o.children)-1]
			o.children = o.children[:len(o.children)-1]
			return
		}
	}
}

func (o *BcsObject) SetOnDestroy(f func()) {
	o.mu.Lock()
	defer o.mu.Unlock()
	o.onDestroy = f
}

func (o *BcsObject) SetParent(newParent *BcsObject) {
	// First, acquire our own lock to detach from old parent
	o.mu.Lock()
	oldParent := o.parent
	o.parent = newParent
	o.mu.Unlock()

	// Carefully detach from old parent using its internal method
	if oldParent != nil && oldParent != newParent {
		oldParent.mu.Lock()
		oldParent.removeChild(o)
		oldParent.mu.Unlock()
	}

	// Carefully attach to new parent
	if newParent != nil {
		newParent.mu.Lock()
		newParent.addChild(o)
		newParent.mu.Unlock()
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

	// Call the subclass destructor hook if defined
	if o.onDestroy != nil {
		o.onDestroy()
	}

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
