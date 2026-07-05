use std::sync::{Arc, Mutex, RwLock, Weak};
use std::cell::RefCell;
use super::bcs_event::{BcsEventT, EventType};
use super::bcs_event_dispatcher::BcsEventDispatcher;

/// BcsObject provides the object tree hierarchy mapping to QObject
pub struct BcsObject {
    pub object_name: Mutex<String>,
    pub parent: Mutex<Option<Weak<BcsObject>>>,
    pub children: Mutex<Vec<Arc<BcsObject>>>,
    pub is_destroyed: Mutex<bool>,
}

impl BcsObject {
    pub fn new() -> Arc<Self> {
        Arc::new(Self {
            object_name: Mutex::new(String::new()),
            parent: Mutex::new(None),
            children: Mutex::new(Vec::new()),
            is_destroyed: Mutex::new(false),
        })
    }

    pub fn new_with_parent(parent: Arc<BcsObject>) -> Arc<Self> {
        let obj = Arc::new(Self {
            object_name: Mutex::new(String::new()),
            parent: Mutex::new(Some(Arc::downgrade(&parent))),
            children: Mutex::new(Vec::new()),
            is_destroyed: Mutex::new(false),
        });

        parent.add_child(obj.clone());
        obj
    }

    pub fn set_object_name(&self, name: &str) {
        *self.object_name.lock().unwrap() = name.to_string();
    }

    pub fn object_name(&self) -> String {
        self.object_name.lock().unwrap().clone()
    }

    pub fn parent(&self) -> Option<Arc<BcsObject>> {
        if let Some(weak_p) = &*self.parent.lock().unwrap() {
            return weak_p.upgrade();
        }
        None
    }

    pub fn set_parent(self: &Arc<Self>, parent: Option<Arc<BcsObject>>) {
        let old_parent = self.parent();

        *self.parent.lock().unwrap() = parent.as_ref().map(|p| Arc::downgrade(p));

        if let Some(old) = old_parent {
            if let Some(new_p) = &parent {
                if !Arc::ptr_eq(&old, new_p) {
                    old.remove_child(self);
                }
            } else {
                old.remove_child(self);
            }
        }

        if let Some(new_p) = parent {
            new_p.add_child(self.clone());
        }
    }

    pub fn add_child(&self, child: Arc<BcsObject>) {
        let mut children = self.children.lock().unwrap();
        // Prevent duplicates
        if !children.iter().any(|c| Arc::ptr_eq(c, &child)) {
            children.push(child);
        }
    }

    pub fn remove_child(&self, child: &Arc<BcsObject>) {
        let mut children = self.children.lock().unwrap();
        if let Some(pos) = children.iter().position(|c| Arc::ptr_eq(c, child)) {
            children.remove(pos);
        }
    }

    pub fn destroy(&self) {
        let mut destroyed = self.is_destroyed.lock().unwrap();
        if *destroyed {
            return;
        }
        *destroyed = true;

        let children_copy = {
            let mut children = self.children.lock().unwrap();
            let copy = children.clone();
            children.clear();
            copy
        };

        for child in children_copy {
            child.destroy();
        }

        if let Some(p) = self.parent() {
            // Can't easily call self.set_parent(None) here because we need an Arc<Self>.
            // Instead, just remove ourselves from the parent.
        }
        *self.parent.lock().unwrap() = None;
    }

    pub fn event(&self, event: &dyn BcsEventT) -> bool {
        // Base implementation does nothing
        false
    }
}
