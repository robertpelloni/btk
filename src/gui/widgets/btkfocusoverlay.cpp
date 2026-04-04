#include <btkfocusoverlay.h>

#include <qpainter.h>
#include <qshowevent.h>

BtkFocusOverlay::BtkFocusOverlay(QWidget *parent, Qt::WindowFlags flags)
   : QWidget(parent, flags)
{
   setAttribute(Qt::WA_TransparentForMouseEvents);
   setFocusPolicy(Qt::NoFocus);
   setWindowOpacity(0.85);
   resize(sizeHint());
   refreshDiagnostics();
   updateTimer();
}

BtkFocusOverlay::~BtkFocusOverlay()
{
   if (m_timerId != 0) {
      killTimer(m_timerId);
   }
}

void BtkFocusOverlay::setTargetWidget(QWidget *widget)
{
   m_targetWidget = widget;
   refreshDiagnostics();
}

QWidget *BtkFocusOverlay::targetWidget() const
{
   return m_targetWidget;
}

void BtkFocusOverlay::setAutoRefreshEnabled(bool enabled)
{
   if (m_autoRefreshEnabled == enabled) {
      return;
   }

   m_autoRefreshEnabled = enabled;
   updateTimer();
}

bool BtkFocusOverlay::autoRefreshEnabled() const
{
   return m_autoRefreshEnabled;
}

void BtkFocusOverlay::setRefreshInterval(int msec)
{
   if (msec < 1) {
      msec = 1;
   }

   if (m_refreshInterval == msec) {
      return;
   }

   m_refreshInterval = msec;
   updateTimer();
}

int BtkFocusOverlay::refreshInterval() const
{
   return m_refreshInterval;
}

void BtkFocusOverlay::refreshDiagnostics()
{
   m_snapshot = BtkFocusDiagnostics::snapshot();
   m_diagnosticsText = buildDisplayText();
   updateGeometry();
   update();
}

QString BtkFocusOverlay::diagnosticsText() const
{
   return m_diagnosticsText;
}

BtkFocusDiagnosticsSnapshot BtkFocusOverlay::snapshot() const
{
   return m_snapshot;
}

QSize BtkFocusOverlay::sizeHint() const
{
   QStringList lines = m_diagnosticsText.split('\n');
   if (lines.isEmpty()) {
      lines.append(QString("BTK Focus Overlay"));
   }

   int width = 240;
   for (const auto &line : lines) {
      width = qMax(width, fontMetrics().horizontalAdvance(line) + 24);
   }

   const int height = qMax(100, (fontMetrics().height() * lines.size()) + 24);
   return QSize(width, height);
}

void BtkFocusOverlay::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing, true);

   painter.fillRect(rect(), QColor(10, 12, 18, 220));
   painter.setPen(QColor(70, 130, 255));
   painter.drawRect(rect().adjusted(0, 0, -1, -1));

   painter.setPen(QColor(255, 255, 255));
   painter.drawText(rect().adjusted(10, 8, -10, -8), Qt::AlignLeft | Qt::TextWordWrap, m_diagnosticsText);
}

void BtkFocusOverlay::showEvent(QShowEvent *event)
{
   refreshDiagnostics();
   QWidget::showEvent(event);
}

void BtkFocusOverlay::timerEvent(QTimerEvent *event)
{
   if (event->timerId() == m_timerId) {
      refreshDiagnostics();
      return;
   }

   QWidget::timerEvent(event);
}

void BtkFocusOverlay::updateTimer()
{
   if (m_timerId != 0) {
      killTimer(m_timerId);
      m_timerId = 0;
   }

   if (m_autoRefreshEnabled) {
      m_timerId = startTimer(m_refreshInterval);
   }
}

QString BtkFocusOverlay::buildDisplayText() const
{
   QStringList lines;
   lines.append(QString("BTK Focus Overlay"));
   lines.append(QString());
   lines.append(BtkFocusDiagnostics::describeCurrentState());

   if (m_targetWidget) {
      lines.append(QString());
      lines.append(QString("targetWidget=%1").arg(BtkFocusDiagnostics::describeWidgetTreePath(m_targetWidget)));
      lines.append(QString("targetContext=%1").arg(QApplication::btkDescribeWidgetContext(m_targetWidget)));
   }

   return lines.join('\n');
}
