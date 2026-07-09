use std::sync::{Arc, Mutex, Condvar};
use std::collections::VecDeque;
use super::bcs_event::BcsEventT;

pub struct BcsEventDispatcher {
    event_queue: Mutex<VecDeque<Box<dyn BcsEventT>>>,
    cond: Condvar,
    running: Mutex<bool>,
}

impl BcsEventDispatcher {
    pub fn new() -> Self {
        Self {
            event_queue: Mutex::new(VecDeque::new()),
            cond: Condvar::new(),
            running: Mutex::new(false),
        }
    }

    pub fn post_event(&self, event: Box<dyn BcsEventT>) {
        let mut queue = self.event_queue.lock().unwrap();
        queue.push_back(event);
        self.cond.notify_one();
    }

    pub fn process_events(&self) {
        let mut queue = self.event_queue.lock().unwrap();
        while let Some(event) = queue.pop_front() {
            drop(queue);
            self.dispatch_event(event);
            queue = self.event_queue.lock().unwrap();
        }
    }

    pub fn exec(&self) {
        *self.running.lock().unwrap() = true;

        let mut queue = self.event_queue.lock().unwrap();
        loop {
            while queue.is_empty() && *self.running.lock().unwrap() {
                queue = self.cond.wait(queue).unwrap();
            }

            if !*self.running.lock().unwrap() {
                break;
            }

            if let Some(event) = queue.pop_front() {
                drop(queue);
                self.dispatch_event(event);
                queue = self.event_queue.lock().unwrap();
            }
        }
    }

    pub fn stop(&self) {
        let _queue = self.event_queue.lock().unwrap();
        *self.running.lock().unwrap() = false;
        self.cond.notify_all();
    }

    fn dispatch_event(&self, _event: Box<dyn BcsEventT>) {
        // Look up target and route event
    }
}
