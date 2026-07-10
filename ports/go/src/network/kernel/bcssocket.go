package kernel

import (
	"fmt"
	"net"
	"sync"
	kernel "github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

type BcsTcpSocket struct {
	*kernel.BcsObject
	conn net.Conn
	mu   sync.Mutex
}

func NewBcsTcpSocket(parent *kernel.BcsObject) *BcsTcpSocket {
	return &BcsTcpSocket{
		BcsObject: kernel.NewBcsObject(parent),
	}
}

func (s *BcsTcpSocket) ConnectToHost(address string, port uint16) error {
	s.mu.Lock()
	defer s.mu.Unlock()

	if s.conn != nil {
		s.conn.Close()
	}

	conn, err := net.Dial("tcp", fmt.Sprintf("%s:%d", address, port))
	if err != nil {
		return err
	}

	s.conn = conn
	return nil
}

func (s *BcsTcpSocket) Write(data []byte) (int, error) {
	s.mu.Lock()
	defer s.mu.Unlock()

	if s.conn == nil {
		return 0, fmt.Errorf("socket not connected")
	}

	return s.conn.Write(data)
}

func (s *BcsTcpSocket) Read(data []byte) (int, error) {
	s.mu.Lock()
	defer s.mu.Unlock()

	if s.conn == nil {
		return 0, fmt.Errorf("socket not connected")
	}

	return s.conn.Read(data)
}

func (s *BcsTcpSocket) Close() error {
	s.mu.Lock()
	defer s.mu.Unlock()

	if s.conn != nil {
		err := s.conn.Close()
		s.conn = nil
		return err
	}
	return nil
}
