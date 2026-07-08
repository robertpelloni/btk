use std::sync::{Arc, Mutex};
use super::bcs_event_dispatcher::BcsEventDispatcher;
use super::bcs_object::BcsObject;

/// BcsEventLoop models QEventLoop
pub struct BcsEventLoop {
    pub base: Arc<BcsObject>,
    pub dispatcher: Arc<BcsEventDispatcher>,
    is_running: Mutex<bool>,
    quit_code: Mutex<i32>,
}

impl BcsEventLoop {
    pub fn new(dispatcher: Arc<BcsEventDispatcher>, parent: Option<Arc<BcsObject>>) -> Self {
        let base = if let Some(p) = parent {
            BcsObject::new_with_parent(p)
        } else {
            BcsObject::new()
        };

        Self {
            base,
            dispatcher,
            is_running: Mutex::new(false),
            quit_code: Mutex::new(0),
        }
    }

    pub fn exec(&self) -> i32 {
        {
            let mut running = self.is_running.lock().unwrap();
            if *running {
                return -1;
            }
            *running = true;
        }

        self.dispatcher.exec();

        let mut running = self.is_running.lock().unwrap();
        *running = false;

        *self.quit_code.lock().unwrap()
    }

    pub fn exit(&self, return_code: i32) {
        *self.quit_code.lock().unwrap() = return_code;
        self.dispatcher.stop();
    }

    pub fn quit(&self) {
        self.exit(0);
    }

    pub fn is_running(&self) -> bool {
        *self.is_running.lock().unwrap()
    }

    pub fn object(&self) -> Arc<BcsObject> {
        self.base.clone()
    }
}
