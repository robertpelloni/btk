using System;
using Bcs.Core.Kernel;

namespace Bcs.Network.Kernel
{
    public class BcsHostAddress : BcsObject
    {
        private string _address;
        private bool _isIPv6;
        private readonly object _lock = new object();

        public BcsHostAddress(string address) : base(null)
        {
            _address = address;
        }

        public override string ToString()
        {
            lock (_lock) { return _address; }
        }
    }

    public enum ProxyType
    {
        DefaultProxy = 0,
        Socks5Proxy,
        NoProxy,
        HttpProxy,
        HttpCachingProxy,
        FtpCachingProxy
    }

    public class BcsNetworkProxy : BcsObject
    {
        private ProxyType _proxyType = ProxyType.DefaultProxy;
        private string _host = string.Empty;
        private int _port = 0;
        private readonly object _lock = new object();

        public BcsNetworkProxy() : base(null) {}

        public ProxyType Type
        {
            get { lock (_lock) { return _proxyType; } }
            set { lock (_lock) { _proxyType = value; } }
        }

        public string HostName
        {
            get { lock (_lock) { return _host; } }
            set { lock (_lock) { _host = value; } }
        }

        public int Port
        {
            get { lock (_lock) { return _port; } }
            set { lock (_lock) { _port = value; } }
        }
    }

    public class BcsNetworkManager : BcsObject
    {
        private static readonly Lazy<BcsNetworkManager> _instance = new Lazy<BcsNetworkManager>(() => new BcsNetworkManager());
        public static BcsNetworkManager Instance => _instance.Value;

        private BcsNetworkProxy _applicationProxy;
        private readonly object _lock = new object();

        private BcsNetworkManager() : base(null) {}

        public BcsNetworkProxy ApplicationProxy
        {
            get { lock (_lock) { return _applicationProxy; } }
            set { lock (_lock) { _applicationProxy = value; } }
        }
    }
}
