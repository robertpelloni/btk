use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;

pub struct BcsCanvas {
    pub base: Arc<BcsObject>,
    width: Mutex<i32>,
    height: Mutex<i32>,
}

impl BcsCanvas {
    pub fn new(parent: Option<Arc<BcsObject>>, width: i32, height: i32) -> Arc<Self> {
        let base = if let Some(p) = parent {
            BcsObject::new_with_parent(p)
        } else {
            BcsObject::new()
        };

        Arc::new(Self {
            base,
            width: Mutex::new(width),
            height: Mutex::new(height),
        })
    }

    pub fn size(&self) -> (i32, i32) {
        (*self.width.lock().unwrap(), *self.height.lock().unwrap())
    }
}
