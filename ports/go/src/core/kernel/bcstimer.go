package kernel

import (
	"sync"
	"time"
)

type BcsTimer struct {
	*BcsObject
	interval     time.Duration
	singleShot   bool
	isActive     bool
	ticker       *time.Ticker
	timer        *time.Timer
	stopChan     chan struct{}
	mu           sync.Mutex
	timerId      int
	timeoutFunc  func()
}

var globalTimerIdCounter int
var timerIdMu sync.Mutex

func getNextTimerId() int {
	timerIdMu.Lock()
	defer timerIdMu.Unlock()
	globalTimerIdCounter++
	return globalTimerIdCounter
}

func NewBcsTimer(parent *BcsObject) *BcsTimer {
	return &BcsTimer{
		BcsObject:  NewBcsObject(parent),
		interval:   0,
		singleShot: false,
		isActive:   false,
		timerId:    getNextTimerId(),
	}
}

func (t *BcsTimer) SetInterval(msec int) {
	t.mu.Lock()
	defer t.mu.Unlock()
	t.interval = time.Duration(msec) * time.Millisecond
}

func (t *BcsTimer) SetSingleShot(singleShot bool) {
	t.mu.Lock()
	defer t.mu.Unlock()
	t.singleShot = singleShot
}

func (t *BcsTimer) IsActive() bool {
	t.mu.Lock()
	defer t.mu.Unlock()
	return t.isActive
}

func (t *BcsTimer) ConnectTimeout(f func()) {
	t.mu.Lock()
	defer t.mu.Unlock()
	t.timeoutFunc = f
}

func (t *BcsTimer) Start(msec int) {
	if msec > 0 {
		t.SetInterval(msec)
	}

	t.mu.Lock()
	if t.isActive {
		t.mu.Unlock()
		t.Stop()
		t.mu.Lock()
	}

	t.isActive = true
	t.stopChan = make(chan struct{})
	t.mu.Unlock()

	go t.runLoop()
}

func (t *BcsTimer) Stop() {
	t.mu.Lock()
	defer t.mu.Unlock()

	if !t.isActive {
		return
	}

	t.isActive = false
	close(t.stopChan)
}

func (t *BcsTimer) runLoop() {
	t.mu.Lock()
	interval := t.interval
	singleShot := t.singleShot
	stopChan := t.stopChan
	timeoutFunc := t.timeoutFunc
	timerId := t.timerId
	t.mu.Unlock()

	if singleShot {
		timer := time.NewTimer(interval)
		select {
		case <-timer.C:
			t.mu.Lock()
			t.isActive = false
			t.mu.Unlock()
			if timeoutFunc != nil {
				timeoutFunc()
			}
			t.dispatchTimerEvent(timerId)
		case <-stopChan:
			timer.Stop()
		}
	} else {
		ticker := time.NewTicker(interval)
		for {
			select {
			case <-ticker.C:
				if timeoutFunc != nil {
					timeoutFunc()
				}
				t.dispatchTimerEvent(timerId)
			case <-stopChan:
				ticker.Stop()
				return
			}
		}
	}
}

func (t *BcsTimer) dispatchTimerEvent(timerId int) {
	// Post BcsTimerEvent to target (which is this timer object)
	event := NewBcsTimerEvent(timerId)
	if k := GetKernel(); k.eventLoop != nil {
		k.eventLoop.dispatcher.PostEvent(event)
	}
}

// BcsCoreApplication models QCoreApplication for basic lifecycle
type BcsCoreApplication struct {
	*BcsObject
	kernel *BcsKernel
}

var coreAppInstance *BcsCoreApplication
var coreAppOnce sync.Once

func GetCoreApplication() *BcsCoreApplication {
	coreAppOnce.Do(func() {
		coreAppInstance = &BcsCoreApplication{
			BcsObject: NewBcsObject(nil),
			kernel:    GetKernel(),
		}
	})
	return coreAppInstance
}

func (a *BcsCoreApplication) Exec() int {
	return a.kernel.Exec()
}

func (a *BcsCoreApplication) Quit() {
	a.kernel.Shutdown()
}
