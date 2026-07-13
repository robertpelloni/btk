package kernel

import (
	"os/exec"
	"sync"
)

type BcsProcess struct {
	*BcsObject
	cmd *exec.Cmd
	mu  sync.Mutex
}

func NewBcsProcess(parent *BcsObject) *BcsProcess {
	return &BcsProcess{
		BcsObject: NewBcsObject(parent),
	}
}

func (p *BcsProcess) Start(command string, args ...string) error {
	p.mu.Lock()
	defer p.mu.Unlock()

	p.cmd = exec.Command(command, args...)
	return p.cmd.Start()
}

func (p *BcsProcess) Terminate() error {
	p.mu.Lock()
	defer p.mu.Unlock()

	if p.cmd != nil && p.cmd.Process != nil {
		return p.cmd.Process.Kill()
	}
	return nil
}

func (p *BcsProcess) Wait() error {
	p.mu.Lock()
	cmd := p.cmd
	p.mu.Unlock()

	if cmd != nil {
		return cmd.Wait()
	}
	return nil
}
