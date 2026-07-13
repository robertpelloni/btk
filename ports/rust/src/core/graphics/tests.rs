#[cfg(test)]
mod tests {
    use std::sync::Arc;
    use crate::core::kernel::bcs_object::BcsObject;
    use crate::core::graphics::bcs_canvas::BcsCanvas;
    use crate::core::graphics::bcs_painter::BcsPainter;

    #[test]
    fn test_graphics_lifecycle() {
        let parent = BcsObject::new();
        let canvas = BcsCanvas::new(Some(parent.clone()), 800, 600);
        let painter = BcsPainter::new(Some(parent.clone()));

        assert_eq!(Arc::as_ptr(&parent), Arc::as_ptr(&canvas.base.parent().unwrap()));
        assert_eq!(canvas.size(), (800, 600));

        assert!(painter.begin(canvas.clone()));
        assert!(!painter.begin(canvas.clone())); // already active

        painter.end();
    }
}
