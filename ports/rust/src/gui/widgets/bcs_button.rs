use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;
use crate::core::kernel::bcs_event::BcsEventT;
use crate::gui::widgets::bcs_widget::{BcsWidget, BcsWidgetImpl};

struct BcsButtonImpl {
    on_click: Mutex<Option<Box<dyn Fn() + Send + Sync>>>,
}

impl BcsWidgetImpl for BcsButtonImpl {
    fn mouse_release_event(&self, _event: &dyn BcsEventT) {
        if let Some(cb) = &*self.on_click.lock().unwrap() {
            cb();
        }
    }
}

pub struct BcsButton {
    pub base: Arc<BcsWidget>,
    text: Mutex<String>,
    on_click_ref: Arc<Mutex<Option<Box<dyn Fn() + Send + Sync>>>>,
}

impl BcsButton {
    pub fn new(parent: Option<Arc<BcsObject>>) -> Arc<Self> {
        let base = BcsWidget::new(parent);

        let on_click_ref: Arc<Mutex<Option<Box<dyn Fn() + Send + Sync>>>> = Arc::new(Mutex::new(None));

        let button_impl = BcsButtonImpl {
            on_click: Mutex::new(None),
        };

        base.set_impl(Box::new(button_impl));

        Arc::new(Self {
            base,
            text: Mutex::new(String::new()),
            on_click_ref,
        })
    }

    pub fn set_text(&self, text: &str) {
        *self.text.lock().unwrap() = text.to_string();
    }

    pub fn text(&self) -> String {
        self.text.lock().unwrap().clone()
    }

    pub fn connect_clicked<F>(&self, callback: F)
    where
        F: Fn() + Send + Sync + 'static,
    {
        // Re-implement the delegate because we lost the reference to the internal struct due to Box<dyn>
        let button_impl = BcsButtonImpl {
            on_click: Mutex::new(Some(Box::new(callback))),
        };
        self.base.set_impl(Box::new(button_impl));
    }
}
