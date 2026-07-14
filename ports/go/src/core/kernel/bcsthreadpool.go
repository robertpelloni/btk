package kernel

import (
	"sync"
)

type BcsThreadPool struct {
	*BcsObject
	tasks   chan func()
	workers int
	wg      sync.WaitGroup
	quit    chan struct{}
	mu      sync.Mutex
	closed  bool
}

func NewBcsThreadPool(parent *BcsObject, numWorkers int) *BcsThreadPool {
	tp := &BcsThreadPool{
		BcsObject: NewBcsObject(parent),
		tasks:     make(chan func(), 100), // Buffer size for queued tasks
		workers:   numWorkers,
		quit:      make(chan struct{}),
		closed:    false,
	}

	tp.startWorkers()
	return tp
}

func (tp *BcsThreadPool) startWorkers() {
	tp.mu.Lock()
	defer tp.mu.Unlock()

	for i := 0; i < tp.workers; i++ {
		tp.wg.Add(1)
		go func() {
			defer tp.wg.Done()
			for task := range tp.tasks {
				task()
			}
		}()
	}
}

func (tp *BcsThreadPool) Submit(task func()) {
	tp.mu.Lock()
	defer tp.mu.Unlock()

	if !tp.closed {
		tp.tasks <- task
	}
}

func (tp *BcsThreadPool) Shutdown() {
	tp.mu.Lock()
	if tp.closed {
		tp.mu.Unlock()
		return
	}
	tp.closed = true
	close(tp.tasks)
	tp.mu.Unlock()

	// Wait outside the lock to prevent deadlocks
	tp.wg.Wait()
}
