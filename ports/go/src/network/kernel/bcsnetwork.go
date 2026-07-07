package kernel

import (
	"sync"
	"github.com/robertpelloni/btk-go/ports/go/src/core/kernel"
)

// Maps to QHostAddress
type BcsHostAddress struct {
	*kernel.BcsObject
	address string
	isIPv6  bool
}

func NewBcsHostAddress(addr string) *BcsHostAddress {
	return &BcsHostAddress{
		BcsObject: kernel.NewBcsObject(nil),
		address:   addr,
	}
}

func (a *BcsHostAddress) ToString() string {
	return a.address
}

// Maps to QNetworkProxy
type ProxyType int
const (
	DefaultProxy ProxyType = iota
	Socks5Proxy
	NoProxy
	HttpProxy
	HttpCachingProxy
	FtpCachingProxy
)

type BcsNetworkProxy struct {
	*kernel.BcsObject
	proxyType ProxyType
	host      string
	port      int
	user      string
	password  string
}

func NewBcsNetworkProxy() *BcsNetworkProxy {
	return &BcsNetworkProxy{
		BcsObject: kernel.NewBcsObject(nil),
		proxyType: DefaultProxy,
	}
}

func (p *BcsNetworkProxy) SetType(t ProxyType) {
	p.proxyType = t
}

func (p *BcsNetworkProxy) SetHostName(host string) {
	p.host = host
}

func (p *BcsNetworkProxy) SetPort(port int) {
	p.port = port
}

// Global network module manager
type BcsNetworkManager struct {
	*kernel.BcsObject
	applicationProxy *BcsNetworkProxy
	mu               sync.RWMutex
}

var netInstance *BcsNetworkManager
var netOnce sync.Once

func GetNetworkManager() *BcsNetworkManager {
	netOnce.Do(func() {
		netInstance = &BcsNetworkManager{
			BcsObject: kernel.NewBcsObject(nil),
		}
	})
	return netInstance
}

func (m *BcsNetworkManager) SetApplicationProxy(proxy *BcsNetworkProxy) {
	m.mu.Lock()
	defer m.mu.Unlock()
	m.applicationProxy = proxy
}

func (m *BcsNetworkManager) ApplicationProxy() *BcsNetworkProxy {
	m.mu.RLock()
	defer m.mu.RUnlock()
	return m.applicationProxy
}
