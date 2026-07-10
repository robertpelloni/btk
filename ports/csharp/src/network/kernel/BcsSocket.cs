using System;
using System.Net.Sockets;
using Bcs.Core.Kernel;

namespace Bcs.Network.Kernel
{
    public class BcsTcpSocket : BcsObject
    {
        private TcpClient _client;
        private NetworkStream _stream;
        private readonly object _lock = new object();

        public BcsTcpSocket(BcsObject parent = null) : base(parent)
        {
        }

        public void ConnectToHost(string address, int port)
        {
            lock (_lock)
            {
                _client?.Close();
                _client = new TcpClient();
                _client.Connect(address, port);
                _stream = _client.GetStream();
            }
        }

        public int Write(byte[] data)
        {
            lock (_lock)
            {
                if (_stream == null || !_client.Connected)
                    throw new InvalidOperationException("Socket not connected.");

                _stream.Write(data, 0, data.Length);
                return data.Length;
            }
        }

        public int Read(byte[] data)
        {
            lock (_lock)
            {
                if (_stream == null || !_client.Connected)
                    throw new InvalidOperationException("Socket not connected.");

                return _stream.Read(data, 0, data.Length);
            }
        }

        public void Close()
        {
            lock (_lock)
            {
                _stream?.Close();
                _client?.Close();
                _stream = null;
                _client = null;
            }
        }
    }
}
