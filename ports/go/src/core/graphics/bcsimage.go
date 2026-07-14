package graphics

import (
	"sync"
	core "github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

type BcsImage struct {
	*core.BcsObject
	width  int
	height int
	pixels []byte
	mu     sync.RWMutex
}

func NewBcsImage(parent *core.BcsObject, width, height int) *BcsImage {
	return &BcsImage{
		BcsObject: core.NewBcsObject(parent),
		width:     width,
		height:    height,
		pixels:    make([]byte, width*height*4), // Assuming RGBA
	}
}

func (img *BcsImage) Size() (int, int) {
	img.mu.RLock()
	defer img.mu.RUnlock()
	return img.width, img.height
}

func (img *BcsImage) SetPixel(x, y int, r, g, b, a byte) {
	img.mu.Lock()
	defer img.mu.Unlock()
	if x >= 0 && x < img.width && y >= 0 && y < img.height {
		idx := (y*img.width + x) * 4
		img.pixels[idx] = r
		img.pixels[idx+1] = g
		img.pixels[idx+2] = b
		img.pixels[idx+3] = a
	}
}

func (img *BcsImage) Pixels() []byte {
	img.mu.RLock()
	defer img.mu.RUnlock()
	pixelsCopy := make([]byte, len(img.pixels))
	copy(pixelsCopy, img.pixels)
	return pixelsCopy
}
