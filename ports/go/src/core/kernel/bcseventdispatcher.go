package kernel

import (
	"sync"
)

// BcsEventDispatcher manages the event queue and message loop routing
type BcsEventDispatcher struct {
	eventQueue []BcsEvent
	mu         sync.Mutex
	cond       *sync.Cond
	running    bool
}

func NewBcsEventDispatcher() *BcsEventDispatcher {
	d := &BcsEventDispatcher{}
	d.cond = sync.NewCond(&d.mu)
	return d
}

// PostEvent queues an event asynchronously to be processed in the main loop
func (d *BcsEventDispatcher) PostEvent(e BcsEvent) {
	d.mu.Lock()
	defer d.mu.Unlock()
	d.eventQueue = append(d.eventQueue, e)
	d.cond.Signal() // Wake up the event loop
}

// ProcessEvents processes all currently pending events.
func (d *BcsEventDispatcher) ProcessEvents() {
	d.mu.Lock()
	defer d.mu.Unlock()

	for len(d.eventQueue) > 0 {
		e := d.eventQueue[0]
		d.eventQueue = d.eventQueue[1:]

		// Unlock while processing the event so we don't deadlock if an event handler
		// posts another event.
		d.mu.Unlock()
		d.dispatchEvent(e)
		d.mu.Lock()
	}
}

// Exec starts the main event loop blocking until Stop is called
func (d *BcsEventDispatcher) Exec() {
	d.mu.Lock()
	d.running = true

	for d.running {
		for len(d.eventQueue) == 0 && d.running {
			d.cond.Wait()
		}

		if !d.running {
			break
		}

		e := d.eventQueue[0]
		d.eventQueue = d.eventQueue[1:]

		d.mu.Unlock()
		d.dispatchEvent(e)
		d.mu.Lock()
	}
	d.mu.Unlock()
}

func (d *BcsEventDispatcher) Stop() {
	d.mu.Lock()
	defer d.mu.Unlock()
	d.running = false
	d.cond.Broadcast()
}

// dispatchEvent routes the event to its intended target (mocked for now)
func (d *BcsEventDispatcher) dispatchEvent(e BcsEvent) {
	// In a real framework, this looks up the target QObject / BcsObject and calls event()
}
