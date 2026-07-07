#[cfg(test)]
mod tests {
    use super::super::bcs_input_arbitrator::BcsInputArbitrator;
    use crate::core::kernel::bcs_event::{BcsEvent, EventType};
    use crate::gui::widgets::bcs_widget::BcsWidget;
    use std::sync::Arc;

    #[test]
    fn test_widget_event_routing() {
        let arbitrator = BcsInputArbitrator::instance();
        let widget = BcsWidget::new(None);
        let event = Box::new(BcsEvent::new(EventType::MouseButtonPress));

        let handled = arbitrator.route_event(&*event, Some(widget));

        // Because rust route_event mock returns false currently
        assert_eq!(handled, false);
    }
}
