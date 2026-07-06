use std::sync::{Arc, Mutex};
use crate::gui::widgets::bcs_widget::BcsWidget;

pub struct BcsLabel {
    pub base: Arc<BcsWidget>,
    text: Mutex<String>,
}

impl BcsLabel {
    pub fn new(parent: Arc<BcsWidget>) -> Arc<Self> {
        Arc::new(Self {
            base: BcsWidget::new(Some(parent.base.clone())),
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
