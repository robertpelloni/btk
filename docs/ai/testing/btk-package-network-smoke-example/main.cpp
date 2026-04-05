#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslSocket>

int main()
{
   QHostAddress address(QString("127.0.0.1"));
   QSslSocket socket;

   return address.isNull()
      || ! address.isLoopback()
      || socket.state() != QAbstractSocket::UnconnectedState
      || ! QSslSocket::supportsSsl() ? 1 : 0;
}
