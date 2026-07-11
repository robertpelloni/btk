#[cfg(test)]
mod tests {
    use std::sync::Arc;
    use crate::core::kernel::bcs_object::BcsObject;
    use crate::network::kernel::bcs_socket::BcsTcpSocket;

    #[test]
    fn test_bcs_tcp_socket_lifecycle() {
        let parent = BcsObject::new();
        let socket = BcsTcpSocket::new(Some(parent.clone()));

        assert_eq!(Arc::as_ptr(&parent), Arc::as_ptr(&socket.base.parent().unwrap()));
    }
}
