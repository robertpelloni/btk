package kernel

import (
	"bytes"
	"io"
	"strings"
	"sync"
)

type BcsTextStream struct {
	mu     sync.Mutex
	reader io.Reader
	writer io.Writer
	buffer *bytes.Buffer
}

func NewBcsTextStream() *BcsTextStream {
	buf := new(bytes.Buffer)
	return &BcsTextStream{
		reader: buf,
		writer: buf,
		buffer: buf,
	}
}

func (s *BcsTextStream) WriteString(text string) {
	s.mu.Lock()
	defer s.mu.Unlock()
	if s.writer != nil {
		s.writer.Write([]byte(text))
	}
}

func (s *BcsTextStream) ReadAll() string {
	s.mu.Lock()
	defer s.mu.Unlock()
	if s.buffer != nil {
		return s.buffer.String()
	}
	if s.reader != nil {
		b, _ := io.ReadAll(s.reader)
		return string(b)
	}
	return ""
}

func (s *BcsTextStream) ReadLine() string {
	s.mu.Lock()
	defer s.mu.Unlock()
	if s.buffer != nil {
		line, _ := s.buffer.ReadString('\n')
		return strings.TrimSuffix(line, "\n")
	}
	return ""
}

func (s *BcsTextStream) Flush() {
	// Not strictly needed for bytes.Buffer
}
