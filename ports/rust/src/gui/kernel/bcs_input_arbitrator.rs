use std::sync::{Arc, Mutex, OnceLock};
use crate::core::kernel::bcs_object::BcsObject;
use crate::core::kernel::bcs_event::BcsEventT;
use crate::gui::widgets::bcs_widget::BcsWidget;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DeviceType {
    Pointer = 0,
    Keyboard,
    Touch,
    Tablet,
    NumDeviceTypes,
}

pub struct BcsInputArbitrator {
    pub base: Arc<BcsObject>,
    device_counts: Mutex<[i32; 4]>,
}

static INSTANCE: OnceLock<Arc<BcsInputArbitrator>> = OnceLock::new();

impl BcsInputArbitrator {
    pub fn instance() -> Arc<BcsInputArbitrator> {
        INSTANCE.get_or_init(|| {
            Arc::new(Self {
                base: BcsObject::new(),
                device_counts: Mutex::new([0; 4]),
            })
        }).clone()
    }

    pub fn device_count(&self, t: DeviceType) -> i32 {
        let counts = self.device_counts.lock().unwrap();
        counts[t as usize]
    }

    pub fn set_device_count(&self, t: DeviceType, count: i32) {
        let mut counts = self.device_counts.lock().unwrap();
        counts[t as usize] = count;
    }

    pub fn route_event(&self, _event: &dyn BcsEventT, _target: Option<Arc<BcsWidget>>) -> bool {
        // Mock routing
        false
    }
}
