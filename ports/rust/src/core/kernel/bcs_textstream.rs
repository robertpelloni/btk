use std::sync::{Arc, Mutex};
use std::io::{Write, Read, Cursor};

pub struct BcsTextStream {
    buffer: Mutex<Cursor<Vec<u8>>>,
}

impl BcsTextStream {
    pub fn new() -> Arc<Self> {
        Arc::new(Self {
            buffer: Mutex::new(Cursor::new(Vec::new())),
        })
    }

    pub fn write_string(&self, text: &str) {
        let mut buf = self.buffer.lock().unwrap();
        let _ = buf.write_all(text.as_bytes());
    }

    pub fn read_all(&self) -> String {
        let mut buf = self.buffer.lock().unwrap();
        let mut result = String::new();
        let pos = buf.position();
        buf.set_position(0);
        let _ = buf.read_to_string(&mut result);
        buf.set_position(pos);
        result
    }

    pub fn flush(&self) {
        let mut buf = self.buffer.lock().unwrap();
        let _ = buf.flush();
    }
}
