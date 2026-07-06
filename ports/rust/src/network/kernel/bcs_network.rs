use std::sync::{Arc, Mutex, OnceLock};
use crate::core::kernel::bcs_object::BcsObject;

pub struct BcsHostAddress {
    pub base: Arc<BcsObject>,
    address: Mutex<String>,
    is_ipv6: Mutex<bool>,
}

impl BcsHostAddress {
    pub fn new(address: &str) -> Arc<Self> {
        Arc::new(Self {
            base: BcsObject::new(),
            address: Mutex::new(address.to_string()),
            is_ipv6: Mutex::new(false),
        })
    }

    pub fn to_string(&self) -> String {
        self.address.lock().unwrap().clone()
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ProxyType {
    DefaultProxy = 0,
    Socks5Proxy,
    NoProxy,
    HttpProxy,
    HttpCachingProxy,
    FtpCachingProxy,
}

pub struct BcsNetworkProxy {
    pub base: Arc<BcsObject>,
    proxy_type: Mutex<ProxyType>,
    host: Mutex<String>,
    port: Mutex<u16>,
}

impl BcsNetworkProxy {
    pub fn new() -> Arc<Self> {
        Arc::new(Self {
            base: BcsObject::new(),
            proxy_type: Mutex::new(ProxyType::DefaultProxy),
            host: Mutex::new(String::new()),
            port: Mutex::new(0),
        })
    }

    pub fn set_type(&self, t: ProxyType) {
        *self.proxy_type.lock().unwrap() = t;
    }

    pub fn set_host_name(&self, host: &str) {
        *self.host.lock().unwrap() = host.to_string();
    }

    pub fn set_port(&self, port: u16) {
        *self.port.lock().unwrap() = port;
    }
}

pub struct BcsNetworkManager {
    pub base: Arc<BcsObject>,
    application_proxy: Mutex<Option<Arc<BcsNetworkProxy>>>,
}

static INSTANCE: OnceLock<Arc<BcsNetworkManager>> = OnceLock::new();

impl BcsNetworkManager {
    pub fn instance() -> Arc<BcsNetworkManager> {
        INSTANCE.get_or_init(|| {
            Arc::new(Self {
                base: BcsObject::new(),
                application_proxy: Mutex::new(None),
            })
        }).clone()
    }

    pub fn set_application_proxy(&self, proxy: Arc<BcsNetworkProxy>) {
        *self.application_proxy.lock().unwrap() = Some(proxy);
    }
}
