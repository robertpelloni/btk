use std::sync::{Arc, Mutex};
use std::net::TcpStream;
use std::io::{Read, Write};
use crate::core::kernel::bcs_object::BcsObject;

pub struct BcsTcpSocket {
    pub base: Arc<BcsObject>,
    stream: Mutex<Option<TcpStream>>,
}

impl BcsTcpSocket {
    pub fn new(parent: Option<Arc<BcsObject>>) -> Arc<Self> {
        let base = if let Some(p) = parent {
            BcsObject::new_with_parent(p)
        } else {
            BcsObject::new()
        };

        Arc::new(Self {
            base,
            stream: Mutex::new(None),
        })
    }

    pub fn connect_to_host(&self, address: &str, port: u16) -> std::io::Result<()> {
        let mut stream_guard = self.stream.lock().unwrap();
        let addr = format!("{}:{}", address, port);
        let stream = TcpStream::connect(addr)?;
        *stream_guard = Some(stream);
        Ok(())
    }

    pub fn write(&self, data: &[u8]) -> std::io::Result<usize> {
        let mut stream_guard = self.stream.lock().unwrap();
        if let Some(stream) = stream_guard.as_mut() {
            stream.write(data)
        } else {
            Err(std::io::Error::new(std::io::ErrorKind::NotConnected, "Socket not connected"))
        }
    }

    pub fn read(&self, data: &mut [u8]) -> std::io::Result<usize> {
        let mut stream_guard = self.stream.lock().unwrap();
        if let Some(stream) = stream_guard.as_mut() {
            stream.read(data)
        } else {
            Err(std::io::Error::new(std::io::ErrorKind::NotConnected, "Socket not connected"))
        }
    }

    pub fn close(&self) {
        let mut stream_guard = self.stream.lock().unwrap();
        *stream_guard = None;
    }
}
