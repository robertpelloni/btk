# BTK Public API Reference

## Module Overview

### Core Types (`<btkcoretypes.h>`)

| BTK Type | Underlying Type | Description |
|----------|----------------|-------------|
| `BTKCoreApplication` | `QCoreApplication` | Event loop and application control |
| `BTKObject` | `QObject` | Base object with signals/slots |
| `BTKTimer` | `QTimer` | Timer events |
| `BTKThread` | `QThread` | Thread control |
| `BTKEventLoop` | `QEventLoop` | Nested event loop |
| `BTKVariant` | `QVariant` | Type-safe value container |
| `BTKString` | `QString` | Unicode string (default: UTF-16) |
| `BTKStringList` | `QStringList` | List of strings |
| `BTKByteArray` | `QByteArray` | Raw byte array |
| `BTKPoint` / `BTKPointF` | `QPoint` / `QPointF` | 2D point |
| `BTKSize` / `BTKSizeF` | `QSize` / `QSizeF` | 2D size |
| `BTKRect` / `BTKRectF` | `QRect` / `QRectF` | 2D rectangle |
| `BTKDate` | `QDate` | Calendar date |
| `BTKTime` | `QTime` | Time of day |
| `BTKDateTime` | `QDateTime` | Date and time combined |
| `BTKUrl` | `QUrl` | URL parser |
| `BTKFile` | `QFile` | File I/O |
| `BTKDir` | `QDir` | Directory operations |
| `BTKSettings` | `QSettings` | Persistent settings |
| `BTKProcess` | `QProcess` | External process control |
| `BTKBuffer` | `QBuffer` | In-memory I/O |
| `BTKRegExp` | `QRegularExpression` | Regular expressions |
| `BTKJsonObject` | `QJsonObject` | JSON object |
| `BTKJsonArray` | `QJsonArray` | JSON array |
| `BTKJsonDocument` | `QJsonDocument` | JSON document |
| `BTKFileSystemWatcher` | `QFileSystemWatcher` | File change monitoring |
| `BTKSharedMemory` | `QSharedMemory` | Shared memory IPC |
| `BTKSystemSemaphore` | `QSystemSemaphore` | System semaphore IPC |

### String Types (`<btkstring.h>`)

| BTK Type | Underlying Type | Description |
|----------|----------------|-------------|
| `BtkChar` | `CsChar` | Unicode character |
| `BtkString` | `CsString` | Default string (UTF-8) |
| `BtkStringUtf8` | `CsString_utf8` | Explicit UTF-8 string |
| `BtkStringUtf16` | `CsString_utf16` | Explicit UTF-16 string |
| `BtkStringView` | `CsStringView` | Non-owning string view |
| `BtkStringViewUtf8` | `CsStringView_utf8` | UTF-8 string view |
| `BtkStringViewUtf16` | `CsStringView_utf16` | UTF-16 string view |
| `BtkBasicString<E,A>` | `CsBasicString<E,A>` | Generic encoded string |
| `BtkBasicStringView<S>` | `CsBasicStringView<S>` | Generic string view |
| `BtkStringIterator<E,A>` | `CsStringIterator<E,A>` | String iterator |
| `BtkStringReverseIterator<T>` | `CsStringReverseIterator<T>` | Reverse iterator |

### Smart Pointer Types (`<btkpointer.h>`)

| BTK Type | Underlying Type | Description |
|----------|----------------|-------------|
| `BtkSharedPointer<T>` | `CsSharedPointer<T>` | Shared ownership pointer |
| `BtkWeakPointer<T>` | `CsWeakPointer<T>` | Weak reference pointer |
| `BtkUniquePointer<T,D>` | `CsUniquePointer<T,D>` | Unique ownership pointer |
| `BtkSharedArrayPointer<T>` | `CsSharedArrayPointer<T>` | Shared array pointer |
| `BtkUniqueArrayPointer<T,D>` | `CsUniqueArrayPointer<T,D>` | Unique array pointer |
| `BtkEnableSharedFromThis<T>` | `CsEnableSharedFromThis<T>` | Enable `shared_from_this()` |
| `BtkPointerTraits<T>` | `CsPointerTraits<T>` | Pointer traits |

### Signal/Slot Types (`<btksignal.h>`)

| BTK Type | Underlying Type | Description |
|----------|----------------|-------------|
| `BtkSignal::SignalBase` | `CsSignal::SignalBase` | Base signal class |
| `BtkSignal::SlotBase` | `CsSignal::SlotBase` | Base slot class |
| `BtkSignal::PendingSlot` | `CsSignal::PendingSlot` | Pending slot invocation |
| `BtkAutoConnection` | `ConnectionKind::AutoConnection` | Auto-select connection type |
| `BtkDirectConnection` | `ConnectionKind::DirectConnection` | Direct (synchronous) call |
| `BtkQueuedConnection` | `ConnectionKind::QueuedConnection` | Queued (async) call |
| `BtkBlockingQueuedConnection` | `ConnectionKind::BlockingQueuedConnection` | Blocking queued call |

### Widget Types (`<btkwidgets.h>`)

| BTK Type | Underlying Type | Description |
|----------|----------------|-------------|
| `BTKWidget` | `QWidget` | Base widget |
| `BTKWindow` | `QMainWindow` | Main window with menus/toolbars |
| `BTKDialog` | `QDialog` | Dialog window |
| `BTKButton` | `QPushButton` | Push button |
| `BTKCheckBox` | `QCheckBox` | Checkbox |
| `BTKRadioButton` | `QRadioButton` | Radio button |
| `BTKToolButton` | `QToolButton` | Toolbar button |
| `BTKLabel` | `QLabel` | Text/image label |
| `BTKLineEdit` | `QLineEdit` | Single-line text input |
| `BTKTextEdit` | `QTextEdit` | Multi-line rich text editor |
| `BTKPlainEdit` | `QPlainTextEdit` | Multi-line plain text editor |
| `BTKFrame` | `QFrame` | Framed container |
| `BTKGroupBox` | `QGroupBox` | Grouped container |
| `BTKTabWidget` | `QTabWidget` | Tabbed container |
| `BTKStackedWidget` | `QStackedWidget` | Stacked container |
| `BTKSplitter` | `QSplitter` | Split panel container |
| `BTKScrollArea` | `QScrollArea` | Scrollable container |
| `BTKListView` | `QListView` | List view (model-based) |
| `BTKTreeView` | `QTreeView` | Tree view (model-based) |
| `BTKTableView` | `QTableView` | Table view (model-based) |
| `BTKListWidget` | `QListWidget` | List widget (item-based) |
| `BTKTreeWidget` | `QTreeWidget` | Tree widget (item-based) |
| `BTKTableWidget` | `QTableWidget` | Table widget (item-based) |
| `BTKComboBox` | `QComboBox` | Dropdown selector |
| `BTKSpinBox` | `QSpinBox` | Integer spin box |
| `BTKDoubleSpinBox` | `QDoubleSpinBox` | Float spin box |
| `BTKSlider` | `QSlider` | Slider control |
| `BTKDial` | `QDial` | Rotary dial |
| `BTKScrollBar` | `QScrollBar` | Scroll bar |
| `BTKProgressBar` | `QProgressBar` | Progress indicator |
| `BTKCalendar` | `QCalendarWidget` | Calendar picker |
| `BTKDateTimeEdit` | `QDateTimeEdit` | Date/time editor |
| `BTKAction` | `QAction` | Menu/toolbar action |
| `BTKMenuBar` | `QMenuBar` | Menu bar |
| `BTKMenu` | `QMenu` | Popup/dropdown menu |
| `BTKToolBar` | `QToolBar` | Tool bar |
| `BTKStatusBar` | `QStatusBar` | Status bar |
| `BTKMessageBox` | `QMessageBox` | Message dialog |
| `BTKFileDialog` | `QFileDialog` | File open/save dialog |
| `BTKColorDialog` | `QColorDialog` | Color picker dialog |
| `BTKFontDialog` | `QFontDialog` | Font picker dialog |
| `BTKInputDialog` | `QInputDialog` | Input dialog |
| `BTKProgressDialog` | `QProgressDialog` | Progress dialog |
| `BTKWizard` | `QWizard` | Wizard dialog |

### Layout Types (`<btklayouts.h>`)

| BTK Type | Underlying Type | Description |
|----------|----------------|-------------|
| `BTKLayout` | `QLayout` | Base layout |
| `BTKHBoxLayout` | `QHBoxLayout` | Horizontal box layout |
| `BTKVBoxLayout` | `QVBoxLayout` | Vertical box layout |
| `BTKGridLayout` | `QGridLayout` | Grid layout |
| `BTKFormLayout` | `QFormLayout` | Form layout (label-field pairs) |
| `BTKStackedLayout` | `QStackedLayout` | Stacked layout |
| `BTKSizePolicy` | `QSizePolicy` | Size policy |

### Painting Types (`<btkpainting.h>`)

| BTK Type | Underlying Type | Description |
|----------|----------------|-------------|
| `BTKPainter` | `QPainter` | 2D painting context |
| `BTKColor` | `QColor` | RGB/ARGB color |
| `BTKPen` | `QPen` | Line/outline style |
| `BTKBrush` | `QBrush` | Fill pattern |
| `BTKFont` | `QFont` | Font description |
| `BTKFontMetrics` | `QFontMetrics` | Font measurement |
| `BTKImage` | `QImage` | Raster image |
| `BTKPixmap` | `QPixmap` | Display-optimized image |
| `BTKIcon` | `QIcon` | Scalable icon |
| `BTKBitmap` | `QBitmap` | Monochrome bitmap |
| `BTKRegion` | `QRegion` | Clip region |
| `BTKPolygon` / `BTKPolygonF` | `QPolygon` / `QPolygonF` | Polygon |
| `BTKTransform` | `QTransform` | 2D affine transform |
| `BTKGradient` | `QGradient` | Gradient fill |
| `BTKPath` | `QPainterPath` | Vector path |

### Network Types (`<btknetworktypes.h>`)

| BTK Type | Underlying Type | Description |
|----------|----------------|-------------|
| `BTKTcpSocket` | `QTcpSocket` | TCP client socket |
| `BTKTcpServer` | `QTcpServer` | TCP server |
| `BTKUdpSocket` | `QUdpSocket` | UDP socket |
| `BTKSslSocket` | `QSslSocket` | SSL/TLS socket |
| `BTKNetworkManager` | `QNetworkAccessManager` | HTTP network manager |
| `BTKNetworkRequest` | `QNetworkRequest` | HTTP request |
| `BTKNetworkReply` | `QNetworkReply` | HTTP response |
| `BTKSslConfig` | `QSslConfiguration` | SSL configuration |
| `BTKHostInfo` | `QHostInfo` | DNS lookup |
| `BTKNetworkInterface` | `QNetworkInterface` | Network interface |
| `BTKNetworkProxy` | `QNetworkProxy` | Network proxy |

### SQL Types (`<btksqltypes.h>`)

| BTK Type | Underlying Type | Description |
|----------|----------------|-------------|
| `BTKDatabase` | `QSqlDatabase` | Database connection |
| `BTKQuery` | `QSqlQuery` | SQL query |
| `BTKSqlError` | `QSqlError` | SQL error |
| `BTKSqlRecord` | `QSqlRecord` | SQL record |
| `BTKSqlField` | `QSqlField` | SQL field |
| `BTKSqlTableModel` | `QSqlTableModel` | Editable SQL table model |
| `BTKSqlQueryModel` | `QSqlQueryModel` | Read-only SQL query model |

### Multi-User Ownership (`<btkinputowner.h>`, `<btkfocustoken.h>`)

| Type | Description |
|------|-------------|
| `BtkInputOwner` | Represents a local or remote user with capabilities |
| `BtkFocusToken` | Owner-scoped focus token |
| `BtkInputArbitrator` | Resolves competing ownership claims |
| `BTKFocusOverlay` | Developer focus/ownership overlay |
| `BTKFocusDiagnostics` | Diagnostic snapshot of focus state |

## Usage Patterns

### Minimal Core Application
```cpp
#include <btkcore.h>

int main(int argc, char *argv[]) {
   BTKCoreApplication app(argc, argv);
   BTKTimer::singleShot(1000, &app, &BTKCoreApplication::quit);
   return app.exec();
}
```

### GUI Application with Widgets
```cpp
#include <btkgui.h>

int main(int argc, char *argv[]) {
   QApplication app(argc, argv);

   BTKWindow window;
   auto *button = new BTKButton("Click Me", &window);

   window.show();
   return app.exec();
}
```

### Network Request
```cpp
#include <btknetworktypes.h>

BTKNetworkManager manager;
BTKNetworkRequest request(BTKUrl("https://example.com/api"));
auto *reply = manager.get(request);
```

### Database Access
```cpp
#include <btksqltypes.h>

BTKDatabase db = BTKDatabase::addDatabase("QPSQL");
db.setHostName("localhost");
db.setDatabaseName("mydb");
db.open();

BTKQuery query(db);
query.exec("SELECT * FROM users");
```

### Multi-User Ownership
```cpp
#include <btkinputowner.h>
#include <btkfocustoken.h>

BtkInputOwner user1("user-alice", "session-1", "Alice");
user1.setCapabilities(BtkInputOwner::PointerCapability |
                      BtkInputOwner::KeyboardCapability);

BtkFocusToken token("focus-doc1", "user-alice", "surface-main");
token.setScope(BtkFocusToken::Scope::Window);
```
