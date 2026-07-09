use std::sync::{Arc, Mutex};
use crate::core::kernel::bcs_object::BcsObject;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PenStyle {
    NoPen = 0,
    SolidLine,
    DashLine,
    DotLine,
    DashDotLine,
    DashDotDotLine,
    CustomDashLine,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PenCapStyle {
    FlatCap = 0,
    SquareCap,
    RoundCap,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PenJoinStyle {
    MiterJoin = 0,
    BevelJoin,
    RoundJoin,
    SvgMiterJoin,
}

pub struct BcsPen {
    pub base: Arc<BcsObject>,
    style: Mutex<PenStyle>,
    color: Mutex<String>,
    width: Mutex<f64>,
    cap_style: Mutex<PenCapStyle>,
    join_style: Mutex<PenJoinStyle>,
}

impl BcsPen {
    pub fn new() -> Arc<Self> {
        Arc::new(Self {
            base: BcsObject::new(),
            style: Mutex::new(PenStyle::SolidLine),
            color: Mutex::new(String::from("#000000")),
            width: Mutex::new(1.0),
            cap_style: Mutex::new(PenCapStyle::SquareCap),
            join_style: Mutex::new(PenJoinStyle::BevelJoin),
        })
    }

    pub fn set_style(&self, style: PenStyle) {
        *self.style.lock().unwrap() = style;
    }

    pub fn style(&self) -> PenStyle {
        *self.style.lock().unwrap()
    }

    pub fn set_color(&self, color: &str) {
        *self.color.lock().unwrap() = color.to_string();
    }

    pub fn color(&self) -> String {
        self.color.lock().unwrap().clone()
    }

    pub fn set_width(&self, width: f64) {
        *self.width.lock().unwrap() = width;
    }

    pub fn width(&self) -> f64 {
        *self.width.lock().unwrap()
    }

    pub fn set_cap_style(&self, cap: PenCapStyle) {
        *self.cap_style.lock().unwrap() = cap;
    }

    pub fn cap_style(&self) -> PenCapStyle {
        *self.cap_style.lock().unwrap()
    }

    pub fn set_join_style(&self, join: PenJoinStyle) {
        *self.join_style.lock().unwrap() = join;
    }

    pub fn join_style(&self) -> PenJoinStyle {
        *self.join_style.lock().unwrap()
    }
}
