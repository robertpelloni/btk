use std::sync::{Arc, Mutex};
use super::bcs_object::BcsObject;

pub struct BcsObjectCleanupHandler {
    pub base: Arc<BcsObject>,
    cleanup_objects: Mutex<Vec<Arc<BcsObject>>>,
}

impl BcsObjectCleanupHandler {
    pub fn new() -> Arc<Self> {
        Arc::new(Self {
            base: BcsObject::new(),
            cleanup_objects: Mutex::new(Vec::new()),
        })
    }

    pub fn add(&self, obj: Arc<BcsObject>) -> Arc<BcsObject> {
        let mut objects = self.cleanup_objects.lock().unwrap();
        if !objects.iter().any(|o| Arc::ptr_eq(o, &obj)) {
            objects.push(obj.clone());
        }
        obj
    }

    pub fn remove(&self, obj: &Arc<BcsObject>) {
        let mut objects = self.cleanup_objects.lock().unwrap();
        if let Some(pos) = objects.iter().position(|o| Arc::ptr_eq(o, obj)) {
            objects.remove(pos);
        }
    }

    pub fn is_empty(&self) -> bool {
        self.cleanup_objects.lock().unwrap().is_empty()
    }

    pub fn clear(&self) {
        let objects = {
            let mut objs = self.cleanup_objects.lock().unwrap();
            let copy = objs.clone();
            objs.clear();
            copy
        };

        for obj in objects {
            obj.destroy();
        }
    }
}
