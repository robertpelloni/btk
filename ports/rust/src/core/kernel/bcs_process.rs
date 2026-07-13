use std::sync::{Arc, Mutex};
use std::process::{Child, Command};
use crate::core::kernel::bcs_object::BcsObject;

pub struct BcsProcess {
    pub base: Arc<BcsObject>,
    child: Mutex<Option<Child>>,
}

impl BcsProcess {
    pub fn new(parent: Option<Arc<BcsObject>>) -> Arc<Self> {
        let base = if let Some(p) = parent {
            BcsObject::new_with_parent(p)
        } else {
            BcsObject::new()
        };

        Arc::new(Self {
            base,
            child: Mutex::new(None),
        })
    }

    pub fn start(&self, command: &str, args: &[&str]) -> std::io::Result<()> {
        let child_process = Command::new(command)
            .args(args)
            .spawn()?;

        *self.child.lock().unwrap() = Some(child_process);
        Ok(())
    }

    pub fn terminate(&self) -> std::io::Result<()> {
        let mut child_guard = self.child.lock().unwrap();
        if let Some(child) = child_guard.as_mut() {
            child.kill()
        } else {
            Ok(())
        }
    }

    pub fn wait(&self) -> std::io::Result<Option<std::process::ExitStatus>> {
        // In Rust, Child.wait() requires mut access and blocks.
        // We take ownership to avoid deadlocking with terminate().
        let mut child = self.child.lock().unwrap().take();
        if let Some(mut c) = child {
            let res = c.wait().map(Some);
            // Put it back in case we need to read status later
            *self.child.lock().unwrap() = Some(c);
            res
        } else {
            Ok(None)
        }
    }
}
