use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;
use crate::core::kernel::bcs_event::{BcsEventT, EventType};

pub trait BcsWidgetImpl: Send + Sync {
    fn mouse_press_event(&self, _event: &dyn BcsEventT) {}
    fn mouse_release_event(&self, _event: &dyn BcsEventT) {}
    fn key_press_event(&self, _event: &dyn BcsEventT) {}
    fn key_release_event(&self, _event: &dyn BcsEventT) {}
    fn paint_event(&self, _event: &dyn BcsEventT) {}
}

pub struct BcsWidgetBaseImpl;
impl BcsWidgetImpl for BcsWidgetBaseImpl {}

pub struct BcsWidget {
    pub base: Arc<BcsObject>,
    visible: Mutex<bool>,
    enabled: Mutex<bool>,
    width: Mutex<i32>,
    height: Mutex<i32>,
    x: Mutex<i32>,
    y: Mutex<i32>,
    has_focus: Mutex<bool>,
    impl_delegate: Mutex<Box<dyn BcsWidgetImpl>>,
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
            has_focus: Mutex::new(false),
            impl_delegate: Mutex::new(Box::new(BcsWidgetBaseImpl)),
        })
    }

    pub fn set_impl(&self, delegate: Box<dyn BcsWidgetImpl>) {
        *self.impl_delegate.lock().unwrap() = delegate;
    }

    pub fn show(&self) { *self.visible.lock().unwrap() = true; }
    pub fn hide(&self) { *self.visible.lock().unwrap() = false; }
    pub fn is_visible(&self) -> bool { *self.visible.lock().unwrap() }

    pub fn resize(&self, width: i32, height: i32) {
        *self.width.lock().unwrap() = width;
        *self.height.lock().unwrap() = height;
    }

    pub fn move_to(&self, x: i32, y: i32) {
        *self.x.lock().unwrap() = x;
        *self.y.lock().unwrap() = y;
    }

    pub fn set_enabled(&self, enabled: bool) { *self.enabled.lock().unwrap() = enabled; }
    pub fn is_enabled(&self) -> bool { *self.enabled.lock().unwrap() }

    pub fn set_focus(&self, focus: bool) { *self.has_focus.lock().unwrap() = focus; }
    pub fn has_focus(&self) -> bool { *self.has_focus.lock().unwrap() }

    pub fn event(&self, event: &dyn BcsEventT) -> bool {
        let delegate = self.impl_delegate.lock().unwrap();
        match event.event_type() {
            EventType::MouseButtonPress => {
                delegate.mouse_press_event(event);
                true
            },
            EventType::MouseButtonRelease => {
                delegate.mouse_release_event(event);
                true
            },
            EventType::KeyPress => {
                delegate.key_press_event(event);
                true
            },
            EventType::KeyRelease => {
                delegate.key_release_event(event);
                true
            },
            EventType::Paint => {
                delegate.paint_event(event);
                true
            },
            _ => self.base.event(event),
        }
    }
}
