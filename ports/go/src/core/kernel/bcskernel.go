package kernel

import (
	"sync"
	"fmt"
)

// Module interfaces for lifecycle registration
type BcsModule interface {
	Initialize() error
	Start() error
	Shutdown() error
	Name() string
}

type BcsKernel struct {
	*BcsObject
	modules      []BcsModule
	eventLoop    *BcsEventLoop
	mu           sync.Mutex
	isStarted    bool
	isShuttingDown bool
}

var instance *BcsKernel
var once sync.Once

func GetKernel() *BcsKernel {
	once.Do(func() {
		instance = &BcsKernel{
			BcsObject: NewBcsObject(nil),
			modules:   make([]BcsModule, 0),
		}
		// In Go, we explicitly set the global event loop for the main thread here
		instance.eventLoop = NewBcsEventLoop(instance.BcsObject)
	})
	return instance
}

func (k *BcsKernel) RegisterModule(mod BcsModule) {
	k.mu.Lock()
	defer k.mu.Unlock()
	if !k.isStarted {
		k.modules = append(k.modules, mod)
	}
}

func (k *BcsKernel) Startup() error {
	k.mu.Lock()
	if k.isStarted {
		k.mu.Unlock()
		return nil
	}
	k.isStarted = true
	k.mu.Unlock()

	// Phase 1: Initialize all registered modules
	for _, mod := range k.modules {
		fmt.Printf("BcsKernel: Initializing module %s\n", mod.Name())
		if err := mod.Initialize(); err != nil {
			return fmt.Errorf("module %s failed to initialize: %v", mod.Name(), err)
		}
	}

	// Phase 2: Start all initialized modules
	for _, mod := range k.modules {
		fmt.Printf("BcsKernel: Starting module %s\n", mod.Name())
		if err := mod.Start(); err != nil {
			return fmt.Errorf("module %s failed to start: %v", mod.Name(), err)
		}
	}

	return nil
}

func (k *BcsKernel) Exec() int {
	if err := k.Startup(); err != nil {
		fmt.Printf("BcsKernel Startup Error: %v\n", err)
		return -1
	}

	fmt.Println("BcsKernel: Entering main event loop...")
	code := k.eventLoop.Exec()

	k.Shutdown()
	return code
}

func (k *BcsKernel) Shutdown() {
	k.mu.Lock()
	if k.isShuttingDown {
		k.mu.Unlock()
		return
	}
	k.isShuttingDown = true
	k.mu.Unlock()

	fmt.Println("BcsKernel: Shutting down...")

	// Reverse order shutdown
	for i := len(k.modules) - 1; i >= 0; i-- {
		mod := k.modules[i]
		fmt.Printf("BcsKernel: Shutting down module %s\n", mod.Name())
		mod.Shutdown()
	}

	// Finally destroy object tree
	k.Destroy()
}
