use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;
use crate::gui::widgets::bcs_widget::BcsWidget;

pub struct BcsLayout {
    pub base: Arc<BcsObject>,
    widgets: Mutex<Vec<Arc<BcsWidget>>>,
}

impl BcsLayout {
    pub fn new(parent: Option<Arc<BcsObject>>) -> Arc<Self> {
        let base = if let Some(p) = parent {
            BcsObject::new_with_parent(p)
        } else {
            BcsObject::new()
        };

        Arc::new(Self {
            base,
            widgets: Mutex::new(Vec::new()),
        })
    }

    pub fn add_widget(&self, widget: Arc<BcsWidget>) {
        let mut widgets = self.widgets.lock().unwrap();
        widgets.push(widget);
    }

    pub fn remove_widget(&self, widget: &Arc<BcsWidget>) {
        let mut widgets = self.widgets.lock().unwrap();
        if let Some(pos) = widgets.iter().position(|x| Arc::ptr_eq(x, widget)) {
            widgets.remove(pos);
        }
    }

    pub fn widgets(&self) -> Vec<Arc<BcsWidget>> {
        let widgets = self.widgets.lock().unwrap();
        widgets.clone()
    }
}
