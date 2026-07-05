use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;

pub struct BcsWidget {
    pub base: Arc<BcsObject>,
    visible: Mutex<bool>,
    enabled: Mutex<bool>,
    width: Mutex<i32>,
    height: Mutex<i32>,
    x: Mutex<i32>,
    y: Mutex<i32>,
}

impl BcsWidget {
    pub fn new(parent: Option<Arc<BcsObject>>) -> Arc<Self> {
        let base = if let Some(p) = parent {
            BcsObject::new_with_parent(p)
        } else {
            BcsObject::new()
        };

        Arc::new(Self {
            base,
            visible: Mutex::new(false),
            enabled: Mutex::new(true),
            width: Mutex::new(0),
            height: Mutex::new(0),
            x: Mutex::new(0),
            y: Mutex::new(0),
        })
    }

    pub fn show(&self) {
        *self.visible.lock().unwrap() = true;
    }

    pub fn hide(&self) {
        *self.visible.lock().unwrap() = false;
    }

    pub fn is_visible(&self) -> bool {
        *self.visible.lock().unwrap()
    }

    pub fn resize(&self, width: i32, height: i32) {
        *self.width.lock().unwrap() = width;
        *self.height.lock().unwrap() = height;
    }

    pub fn move_to(&self, x: i32, y: i32) {
        *self.x.lock().unwrap() = x;
        *self.y.lock().unwrap() = y;
    }

    pub fn set_enabled(&self, enabled: bool) {
        *self.enabled.lock().unwrap() = enabled;
    }

    pub fn is_enabled(&self) -> bool {
        *self.enabled.lock().unwrap() = enabled;
    }
}
