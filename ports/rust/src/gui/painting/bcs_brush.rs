use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BrushStyle {
    NoBrush = 0,
    SolidPattern,
    Dense1Pattern,
    Dense2Pattern,
    Dense3Pattern,
    Dense4Pattern,
    Dense5Pattern,
    Dense6Pattern,
    Dense7Pattern,
    HorPattern,
    VerPattern,
    CrossPattern,
    BDiagPattern,
    FDiagPattern,
    DiagCrossPattern,
    LinearGradientPattern,
    RadialGradientPattern,
    ConicalGradientPattern,
    TexturePattern,
}

pub struct BcsBrush {
    pub base: Arc<BcsObject>,
    style: Mutex<BrushStyle>,
    color: Mutex<String>,
}

impl BcsBrush {
    pub fn new() -> Arc<Self> {
        Arc::new(Self {
            base: BcsObject::new(),
            style: Mutex::new(BrushStyle::NoBrush),
            color: Mutex::new(String::from("#000000")),
        })
    }

    pub fn new_with_color_style(color: &str, style: BrushStyle) -> Arc<Self> {
        Arc::new(Self {
            base: BcsObject::new(),
            style: Mutex::new(style),
            color: Mutex::new(color.to_string()),
        })
    }

    pub fn set_style(&self, style: BrushStyle) {
        *self.style.lock().unwrap() = style;
    }

    pub fn style(&self) -> BrushStyle {
        *self.style.lock().unwrap()
    }

    pub fn set_color(&self, color: &str) {
        *self.color.lock().unwrap() = color.to_string();
    }

    pub fn color(&self) -> String {
        self.color.lock().unwrap().clone()
    }
}
