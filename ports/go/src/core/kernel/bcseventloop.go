package kernel

import (
	"sync"
)

// BcsEventLoop models QEventLoop
type BcsEventLoop struct {
	*BcsObject
	dispatcher *BcsEventDispatcher
	mu         sync.Mutex
	isRunning  bool
	quitCode   int
}

func NewBcsEventLoop(parent *BcsObject) *BcsEventLoop {
	return &BcsEventLoop{
		BcsObject:  NewBcsObject(parent),
		dispatcher: NewBcsEventDispatcher(),
		isRunning:  false,
		quitCode:   0,
	}
}

// Exec enters the main event loop and waits until exit() is called
func (l *BcsEventLoop) Exec() int {
	l.mu.Lock()
	if l.isRunning {
		l.mu.Unlock()
		return -1 // Already running
	}
	l.isRunning = true
	l.mu.Unlock()

	// Blocks until Stop is called
	l.dispatcher.Exec()

	l.mu.Lock()
	defer l.mu.Unlock()
	l.isRunning = false
	return l.quitCode
}

// Exit tells the event loop to exit with a return code
func (l *BcsEventLoop) Exit(returnCode int) {
	l.mu.Lock()
	defer l.mu.Unlock()

	l.quitCode = returnCode
	l.dispatcher.Stop()
}

// Quit is a convenience for exit(0)
func (l *BcsEventLoop) Quit() {
	l.Exit(0)
}

func (l *BcsEventLoop) IsRunning() bool {
	l.mu.Lock()
	defer l.mu.Unlock()
	return l.isRunning
}
