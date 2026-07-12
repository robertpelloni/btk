use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;
use crate::gui::widgets::bcs_widget::BcsWidget;

pub struct BcsInput {
    pub base: Arc<BcsWidget>,
    text: Mutex<String>,
}

impl BcsInput {
    pub fn new(parent: Option<Arc<BcsObject>>) -> Arc<Self> {
        let base = BcsWidget::new(parent);
        Arc::new(Self {
            base,
            text: Mutex::new(String::new()),
        })
    }

    pub fn set_text(&self, text: &str) {
        *self.text.lock().unwrap() = text.to_string();
    }

    pub fn text(&self) -> String {
        self.text.lock().unwrap().clone()
    }
}
