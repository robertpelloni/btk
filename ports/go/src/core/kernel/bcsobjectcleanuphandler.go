package kernel

import "sync"

// BcsObjectCleanupHandler maps to QObjectCleanupHandler
type BcsObjectCleanupHandler struct {
	*BcsObject
	cleanupObjects []*BcsObject
	mu             sync.Mutex
}

func NewBcsObjectCleanupHandler() *BcsObjectCleanupHandler {
	return &BcsObjectCleanupHandler{
		BcsObject:      NewBcsObject(nil),
		cleanupObjects: make([]*BcsObject, 0),
	}
}

func (h *BcsObjectCleanupHandler) Add(obj *BcsObject) *BcsObject {
	h.mu.Lock()
	defer h.mu.Unlock()

	for _, o := range h.cleanupObjects {
		if o == obj {
			return obj
		}
	}
	h.cleanupObjects = append(h.cleanupObjects, obj)
	return obj
}

func (h *BcsObjectCleanupHandler) Remove(obj *BcsObject) {
	h.mu.Lock()
	defer h.mu.Unlock()

	for i, o := range h.cleanupObjects {
		if o == obj {
			h.cleanupObjects[i] = h.cleanupObjects[len(h.cleanupObjects)-1]
			h.cleanupObjects = h.cleanupObjects[:len(h.cleanupObjects)-1]
			return
		}
	}
}

func (h *BcsObjectCleanupHandler) IsEmpty() bool {
	h.mu.Lock()
	defer h.mu.Unlock()
	return len(h.cleanupObjects) == 0
}

func (h *BcsObjectCleanupHandler) Clear() {
	h.mu.Lock()
	defer h.mu.Unlock()
	for _, obj := range h.cleanupObjects {
		obj.Destroy()
	}
	h.cleanupObjects = nil
}
