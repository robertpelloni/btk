package kernel

import (
	"sync"
	"fmt"
	"github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

type DeviceType int

const (
	DeviceTypePointer DeviceType = iota
	DeviceTypeKeyboard
	DeviceTypeTouch
	DeviceTypeTablet
	NumDeviceTypes
)

// BcsInputArbitrator manages multiple input streams and resolves multi-user ownership
type BcsInputArbitrator struct {
	*kernel.BcsObject
	deviceCounts [NumDeviceTypes]int
	mu           sync.RWMutex
}

var inputInstance *BcsInputArbitrator
var inputOnce sync.Once

func GetInputArbitrator() *BcsInputArbitrator {
	inputOnce.Do(func() {
		inputInstance = &BcsInputArbitrator{
			BcsObject: kernel.NewBcsObject(nil),
		}
	})
	return inputInstance
}

func (a *BcsInputArbitrator) DeviceCount(t DeviceType) int {
	a.mu.RLock()
	defer a.mu.RUnlock()
	if t >= 0 && t < NumDeviceTypes {
		return a.deviceCounts[t]
	}
	return 0
}

func (a *BcsInputArbitrator) SetDeviceCount(t DeviceType, count int) {
	a.mu.Lock()
	defer a.mu.Unlock()
	if t >= 0 && t < NumDeviceTypes {
		a.deviceCounts[t] = count
		fmt.Printf("BcsInputArbitrator: Device count updated for %d -> %d\n", t, count)
	}
}

// RouteEvent is where the multi-user routing logic will live
func (a *BcsInputArbitrator) RouteEvent(e kernel.BcsEvent, target *BcsWidget) bool {
	// Abstract mapping of events to widget receivers based on focus tokens
	if target != nil {
		return target.Event(e)
	}
	return false
}
