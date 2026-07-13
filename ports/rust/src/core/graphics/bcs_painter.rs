use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;
use super::bcs_canvas::BcsCanvas;

pub struct BcsPainter {
    pub base: Arc<BcsObject>,
    canvas: Mutex<Option<Arc<BcsCanvas>>>,
    active: Mutex<bool>,
}

impl BcsPainter {
    pub fn new(parent: Option<Arc<BcsObject>>) -> Arc<Self> {
        let base = if let Some(p) = parent {
            BcsObject::new_with_parent(p)
        } else {
            BcsObject::new()
        };

        Arc::new(Self {
            base,
            canvas: Mutex::new(None),
            active: Mutex::new(false),
        })
    }

    pub fn begin(&self, canvas: Arc<BcsCanvas>) -> bool {
        let mut active = self.active.lock().unwrap();
        if *active {
            return false;
        }
        *self.canvas.lock().unwrap() = Some(canvas);
        *active = true;
        true
    }

    pub fn end(&self) {
        let mut active = self.active.lock().unwrap();
        *active = false;
        *self.canvas.lock().unwrap() = None;
    }
}
