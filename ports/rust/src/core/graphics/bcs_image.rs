use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;

pub struct BcsImage {
    pub base: Arc<BcsObject>,
    width: Mutex<i32>,
    height: Mutex<i32>,
    pixels: Mutex<Vec<u8>>,
}

impl BcsImage {
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
            pixels: Mutex::new(vec![0; (width * height * 4) as usize]), // RGBA
        })
    }

    pub fn size(&self) -> (i32, i32) {
        (*self.width.lock().unwrap(), *self.height.lock().unwrap())
    }

    pub fn set_pixel(&self, x: i32, y: i32, r: u8, g: u8, b: u8, a: u8) {
        let w = *self.width.lock().unwrap();
        let h = *self.height.lock().unwrap();
        if x >= 0 && x < w && y >= 0 && y < h {
            let mut px = self.pixels.lock().unwrap();
            let idx = ((y * w + x) * 4) as usize;
            px[idx] = r;
            px[idx + 1] = g;
            px[idx + 2] = b;
            px[idx + 3] = a;
        }
    }

    pub fn pixels(&self) -> Vec<u8> {
        self.pixels.lock().unwrap().clone()
    }
}
