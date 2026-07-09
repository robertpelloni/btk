using System;
using System.IO;
using System.Text;

namespace Bcs.Core.Kernel
{
    public class BcsTextStream
    {
        private MemoryStream _memoryStream;
        private StreamReader _reader;
        private StreamWriter _writer;
        private readonly object _lock = new object();

        public BcsTextStream()
        {
            _memoryStream = new MemoryStream();
            _reader = new StreamReader(_memoryStream, Encoding.UTF8);
            _writer = new StreamWriter(_memoryStream, Encoding.UTF8);
        }

        public void WriteString(string text)
        {
            lock (_lock)
            {
                _writer.Write(text);
                _writer.Flush();
            }
        }

        public string ReadAll()
        {
            lock (_lock)
            {
                long pos = _memoryStream.Position;
                _memoryStream.Position = 0;
                string result = _reader.ReadToEnd();
                _memoryStream.Position = pos;
                return result;
            }
        }

        public string ReadLine()
        {
            lock (_lock)
            {
                return _reader.ReadLine();
            }
        }

        public void Flush()
        {
            lock (_lock)
            {
                _writer.Flush();
            }
        }
    }
}
