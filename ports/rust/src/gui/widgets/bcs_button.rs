use std::sync::{Arc, Mutex};
use crate::gui::widgets::bcs_widget::BcsWidget;

pub struct BcsButton {
    pub base: Arc<BcsWidget>,
    text: Mutex<String>,
    on_click: Mutex<Option<Box<dyn Fn() + Send + Sync>>>,
}

impl BcsButton {
    pub fn new(parent: Arc<BcsWidget>) -> Arc<Self> {
        Arc::new(Self {
            base: BcsWidget::new(Some(parent.base.clone())),
            text: Mutex::new(String::new()),
            on_click: Mutex::new(None),
        })
    }

    pub fn set_text(&self, text: &str) {
        *self.text.lock().unwrap() = text.to_string();
    }

    pub fn text(&self) -> String {
        self.text.lock().unwrap().clone()
    }

    pub fn connect_clicked<F>(&self, f: F)
    where
        F: Fn() + Send + Sync + 'static,
    {
        *self.on_click.lock().unwrap() = Some(Box::new(f));
    }
}
