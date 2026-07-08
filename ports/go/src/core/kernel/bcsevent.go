package kernel

import (
	"sync"
)

// EventType enumerates all global event IDs modeled after C++ QEvent::Type
type EventType int

const (
	None EventType = iota
	Timer
	MouseButtonPress
	MouseButtonRelease
	MouseButtonDblClick
	MouseMove
	KeyPress
	KeyRelease
	FocusIn
	FocusOut
	Enter
	Leave
	Paint
	Move
	Resize
	Create
	Destroy
	Show
	Hide
	Close
	Quit
	ApplicationActivate
	ApplicationDeactivate
	// Add other critical event types matching QEvent::Type here
	User = 1000
)

// BcsEvent is the base structure for all UI and System events in the framework.
type BcsEvent interface {
	Type() EventType
	IsSpontaneous() bool
	IsAccepted() bool
	Accept()
	Ignore()
	SetAccepted(bool)
}

type Event struct {
	eventType   EventType
	spontaneous bool
	accepted    bool
	mu          sync.Mutex // For thread-safe event mutation if needed
}

func NewEvent(t EventType) *Event {
	return &Event{
		eventType:   t,
		spontaneous: false,
		accepted:    true,
	}
}

func (e *Event) Type() EventType {
	return e.eventType
}

func (e *Event) IsSpontaneous() bool {
	return e.spontaneous
}

func (e *Event) IsAccepted() bool {
	e.mu.Lock()
	defer e.mu.Unlock()
	return e.accepted
}

func (e *Event) Accept() {
	e.mu.Lock()
	defer e.mu.Unlock()
	e.accepted = true
}

func (e *Event) Ignore() {
	e.mu.Lock()
	defer e.mu.Unlock()
	e.accepted = false
}

func (e *Event) SetAccepted(accept bool) {
	e.mu.Lock()
	defer e.mu.Unlock()
	e.accepted = accept
}

func (e *Event) SetSpontaneous(spontaneous bool) {
	e.spontaneous = spontaneous
}

// BcsTimerEvent maps directly to C++ BcsTimerEvent
type BcsTimerEvent struct {
	*Event
	TimerId int
}

func NewBcsTimerEvent(timerId int) *BcsTimerEvent {
	return &BcsTimerEvent{
		Event:   NewEvent(Timer),
		TimerId: timerId,
	}
}

// BcsChildEvent maps to C++ QChildEvent
type BcsChildEvent struct {
	*Event
	Child interface{} // In Go, QObject maps to generic interface{} for now
}

func NewBcsChildEvent(t EventType, child interface{}) *BcsChildEvent {
	return &BcsChildEvent{
		Event: NewEvent(t),
		Child: child,
	}
}

const (
	ChildAdded EventType = iota + 1000 // Just offset from user for now
	ChildRemoved
	DynamicPropertyChange

)

// BcsDynamicPropertyChangeEvent maps to QDynamicPropertyChangeEvent
type BcsDynamicPropertyChangeEvent struct {
	*Event
	PropertyName []byte
}

func NewBcsDynamicPropertyChangeEvent(name []byte) *BcsDynamicPropertyChangeEvent {
	return &BcsDynamicPropertyChangeEvent{
		Event:        NewEvent(DynamicPropertyChange),
		PropertyName: name,
	}
}

// BcsCustomEvent maps to QCustomEvent
type BcsCustomEvent struct {
	*Event
	Data interface{}
}

func NewBcsCustomEvent(t EventType, data interface{}) *BcsCustomEvent {
	return &BcsCustomEvent{
		Event: NewEvent(t),
		Data:  data,
	}
}
