use std::sync::{Arc, Mutex};

/// EventType enumerates all global event IDs modeled after C++ QEvent::Type
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum EventType {
    None = 0,
    Timer,
    MouseButtonPress,
    MouseButtonRelease,
    MouseButtonDblClick,
    MouseMove,
    KeyPress,
    KeyRelease,
    FocusIn,
    FocusOut,
    Enter,
    Leave,
    Paint,
    Move,
    Resize,
    Create,
    Destroy,
    Show,
    Hide,
    Close,
    Quit,
    ApplicationActivate,
    ApplicationDeactivate,
    DynamicPropertyChange,
    User = 1000,
}

pub trait BcsEventT: Send + Sync {
    fn as_any(&self) -> &dyn std::any::Any;
    fn event_type(&self) -> EventType;
    fn is_spontaneous(&self) -> bool;
    fn is_accepted(&self) -> bool;
    fn accept(&self);
    fn ignore(&self);
    fn set_accepted(&self, accept: bool);
}

pub struct BcsEvent {
    event_type: EventType,
    spontaneous: bool,
    accepted: Mutex<bool>,
}

impl BcsEvent {
    pub fn new(event_type: EventType) -> Self {
        Self {
            event_type,
            spontaneous: false,
            accepted: Mutex::new(true),
        }
    }

    pub fn set_spontaneous(&mut self, spontaneous: bool) {
        self.spontaneous = spontaneous;
    }
}

impl BcsEventT for BcsEvent {
    fn as_any(&self) -> &dyn std::any::Any { self }
    fn event_type(&self) -> EventType {
        self.event_type
    }

    fn is_spontaneous(&self) -> bool {
        self.spontaneous
    }

    fn is_accepted(&self) -> bool {
        *self.accepted.lock().unwrap()
    }

    fn accept(&self) {
        *self.accepted.lock().unwrap() = true;
    }

    fn ignore(&self) {
        *self.accepted.lock().unwrap() = false;
    }

    fn set_accepted(&self, accept: bool) {
        *self.accepted.lock().unwrap() = accept;
    }
}

pub struct BcsTimerEvent {
    base: BcsEvent,
    pub timer_id: i32,
}

impl BcsTimerEvent {
    pub fn new(timer_id: i32) -> Self {
        Self {
            base: BcsEvent::new(EventType::Timer),
            timer_id,
        }
    }
}

impl BcsEventT for BcsTimerEvent {
    fn as_any(&self) -> &dyn std::any::Any { self }
    fn event_type(&self) -> EventType { self.base.event_type() }
    fn is_spontaneous(&self) -> bool { self.base.is_spontaneous() }
    fn is_accepted(&self) -> bool { self.base.is_accepted() }
    fn accept(&self) { self.base.accept() }
    fn ignore(&self) { self.base.ignore() }
    fn set_accepted(&self, accept: bool) { self.base.set_accepted(accept) }
}

pub struct BcsDynamicPropertyChangeEvent {
    base: BcsEvent,
    pub property_name: Vec<u8>,
}

impl BcsDynamicPropertyChangeEvent {
    pub fn new(property_name: Vec<u8>) -> Self {
        Self {
            base: BcsEvent::new(EventType::DynamicPropertyChange),
            property_name,
        }
    }
}

impl BcsEventT for BcsDynamicPropertyChangeEvent {
    fn as_any(&self) -> &dyn std::any::Any { self }
    fn event_type(&self) -> EventType { self.base.event_type() }
    fn is_spontaneous(&self) -> bool { self.base.is_spontaneous() }
    fn is_accepted(&self) -> bool { self.base.is_accepted() }
    fn accept(&self) { self.base.accept() }
    fn ignore(&self) { self.base.ignore() }
    fn set_accepted(&self, accept: bool) { self.base.set_accepted(accept) }
}

pub struct BcsCustomEvent {
    base: BcsEvent,
    pub data: Option<Box<dyn std::any::Any + Send + Sync>>,
}

impl BcsCustomEvent {
    pub fn new(event_type: EventType, data: Option<Box<dyn std::any::Any + Send + Sync>>) -> Self {
        Self {
            base: BcsEvent::new(event_type),
            data,
        }
    }
}

impl BcsEventT for BcsCustomEvent {
    fn as_any(&self) -> &dyn std::any::Any { self }
    fn event_type(&self) -> EventType { self.base.event_type() }
    fn is_spontaneous(&self) -> bool { self.base.is_spontaneous() }
    fn is_accepted(&self) -> bool { self.base.is_accepted() }
    fn accept(&self) { self.base.accept() }
    fn ignore(&self) { self.base.ignore() }
    fn set_accepted(&self, accept: bool) { self.base.set_accepted(accept) }
}
