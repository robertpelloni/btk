// Package core provides fundamental types and utilities for the BTK framework.
// This package contains Go equivalents of the BTK core types.

package core

// Application
type CoreApplication interface{}

// Object model
type Object interface{}
type Timer interface{}
type Thread interface{}
type EventLoop interface{}
type Variant interface{}

// I/O
type File interface{}
type Dir interface{}
type Url interface{}
type Settings interface{}
type Process interface{}
type Buffer interface{}
type ByteArray interface{}
type FileSystemWatcher interface{}

// Data types
type String string
type StringList []string
type Point struct{ X, Y int }
type PointF struct{ X, Y float64 }
type Size struct{ Width, Height int }
type SizeF struct{ Width, Height float64 }
type Rect struct{ X, Y, Width, Height int }
type RectF struct{ X, Y, Width, Height float64 }
type Date interface{}
type Time interface{}
type DateTime interface{}
type Locale interface{}
type MimeData interface{}
type RegExp interface{}

// JSON
type JsonObject map[string]interface{}
type JsonArray []interface{}
type JsonDocument interface{}

// IPC
type SharedMemory interface{}
type SystemSemaphore interface{}