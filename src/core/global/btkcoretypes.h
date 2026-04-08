/***********************************************************************
*
* BTK Core type aliases.
* Clean BTK-branded API surface over core Qt types.
*
***********************************************************************/

#ifndef BTK_CORE_ALIASES_H
#define BTK_CORE_ALIASES_H

#include <qcoreapplication.h>
#include <qobject.h>
#include <qtimer.h>
#include <qthread.h>
#include <qeventloop.h>
#include <qurl.h>
#include <qfile.h>
#include <qdir.h>
#include <qsettings.h>
#include <qvariant.h>
#include <qdatetime.h>
#include <qlocale.h>
#include <qmimedata.h>
#include <qprocess.h>
#include <qbuffer.h>
#include <qbytearray.h>
#include <qpoint.h>
#include <qsize.h>
#include <qrect.h>
#include <qstringlist.h>
#include <qstring.h>
#include <qregularexpression.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qfilesystemwatcher.h>
#include <qsharedmemory.h>
#include <qsystemsemaphore.h>

// Application
using BTKCoreApplication = QCoreApplication;

// Object model
using BTKObject     = QObject;
using BTKTimer      = QTimer;
using BTKThread     = QThread;
using BTKEventLoop  = QEventLoop;
using BTKVariant    = QVariant;

// I/O
using BTKFile       = QFile;
using BTKDir        = QDir;
using BTKUrl        = QUrl;
using BTKSettings   = QSettings;
using BTKProcess    = QProcess;
using BTKBuffer     = QBuffer;
using BTKByteArray  = QByteArray;
using BTKFileSystemWatcher = QFileSystemWatcher;

// Data types
using BTKString     = QString;
using BTKStringList = QStringList;
using BTKPoint      = QPoint;
using BTKPointF     = QPointF;
using BTKSize       = QSize;
using BTKSizeF      = QSizeF;
using BTKRect       = QRect;
using BTKRectF      = QRectF;
using BTKDate       = QDate;
using BTKTime       = QTime;
using BTKDateTime   = QDateTime;
using BTKLocale     = QLocale;
using BTKMimeData   = QMimeData;
using BTKRegExp     = QRegularExpression;

// JSON
using BTKJsonObject   = QJsonObject;
using BTKJsonArray    = QJsonArray;
using BTKJsonDocument = QJsonDocument;

// IPC
using BTKSharedMemory    = QSharedMemory;
using BTKSystemSemaphore = QSystemSemaphore;

#endif
