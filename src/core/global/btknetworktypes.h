/***********************************************************************
*
* BTK Network type aliases.
* Clean BTK-branded API surface over Qt network types.
*
***********************************************************************/

#ifndef BTK_NETWORK_ALIASES_H
#define BTK_NETWORK_ALIASES_H

#include <qtcpsocket.h>
#include <qtcpserver.h>
#include <qudpsocket.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>
#include <qnetworkreply.h>
#include <qsslsocket.h>
#include <qsslconfiguration.h>
#include <qhostinfo.h>
#include <qnetworkinterface.h>
#include <qnetworkproxy.h>

using BTKTcpSocket    = QTcpSocket;
using BTKTcpServer    = QTcpServer;
using BTKUdpSocket    = QUdpSocket;
using BTKSslSocket    = QSslSocket;
using BTKNetworkManager = QNetworkAccessManager;
using BTKNetworkRequest = QNetworkRequest;
using BTKNetworkReply = QNetworkReply;
using BTKSslConfig   = QSslConfiguration;
using BTKHostInfo    = QHostInfo;
using BTKNetworkInterface = QNetworkInterface;
using BTKNetworkProxy = QNetworkProxy;

#endif
