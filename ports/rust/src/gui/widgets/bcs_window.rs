use std::sync::{Arc, Mutex};
use crate::gui::widgets::bcs_widget::BcsWidget;

pub struct BcsWindow {
    pub base: Arc<BcsWidget>,
    title: Mutex<String>,
}

impl BcsWindow {
    pub fn new() -> Arc<Self> {
        Arc::new(Self {
            base: BcsWidget::new(None),
            title: Mutex::new(String::new()),
        })
    }

    pub fn set_title(&self, title: &str) {
        *self.title.lock().unwrap() = title.to_string();
    }

    pub fn title(&self) -> String {
        self.title.lock().unwrap().clone()
    }
}
