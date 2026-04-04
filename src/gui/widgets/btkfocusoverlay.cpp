#include <btkfocusoverlay.h>

#include <qapplication.h>
#include <qevent.h>
#include <qpainter.h>

namespace {
QColor btkOverlayBackgroundColor()
{
   return QColor(10, 12, 18, 232);
}

QColor btkOverlayBorderColor()
{
   return QColor(70, 130, 255, 220);
}

QColor btkOverlayTitleColor()
{
   return QColor(255, 255, 255);
}

QColor btkOverlayLabelColor()
{
   return QColor(145, 175, 255);
}

QColor btkOverlayValueColor()
{
   return QColor(232, 238, 255);
}

QColor btkOverlayAccentForLine(const QString &line)
{
   if (line.contains("decision=Reject") || line.contains("blockingOwner=") && ! line.contains("blockingOwner=<none>")) {
      return QColor(255, 110, 110);
   }

   if (line.contains("decision=Transfer") || line.contains("OwnerExclusive") || line.contains("ApplicationExclusive")
      || line.contains("SystemExclusive") || line.contains("WindowExclusive")) {
      return QColor(255, 196, 92);
   }

   if (line.contains("decision=Share") || line.contains("activePopupOwner=") || line.contains("activeModalOwner=")
      || line.contains("owner=") && line.contains("tokens=")) {
      return QColor(90, 220, 160);
   }

   return QColor(170, 186, 220);
}

int btkWrappedTextHeight(const QFontMetrics &fm, int width, const QString &text)
{
   if (text.isEmpty()) {
      return fm.height();
   }

   return fm.boundingRect(QRect(0, 0, width, 100000), Qt::TextWordWrap, text).height();
}

void btkDrawSectionHeader(QPainter &painter, int x, int &y, int width, const QString &title)
{
   painter.setPen(btkOverlayLabelColor());
   painter.drawText(QRect(x, y, width, painter.fontMetrics().height()), Qt::AlignLeft | Qt::AlignVCenter, title);
   y += painter.fontMetrics().height() + 6;
}

void btkDrawWrappedBlock(QPainter &painter, int x, int &y, int width, const QString &text, const QColor &color)
{
   painter.setPen(color);
   const QRect textRect(x, y, width, btkWrappedTextHeight(painter.fontMetrics(), width, text));
   painter.drawText(textRect, Qt::AlignLeft | Qt::TextWordWrap, text);
   y = textRect.bottom() + 8;
}

void btkDrawChip(QPainter &painter, int x, int y, const QString &label, const QString &value, const QColor &fillColor)
{
   const QString text = label + QString(": ") + value;
   const int textWidth = painter.fontMetrics().horizontalAdvance(text);
   const QRect chipRect(x, y, textWidth + 18, painter.fontMetrics().height() + 10);

   painter.setPen(Qt::NoPen);
   painter.setBrush(fillColor);
   painter.drawRoundedRect(chipRect, 7, 7);

   painter.setPen(QColor(15, 18, 28));
   painter.drawText(chipRect.adjusted(9, 0, -9, 0), Qt::AlignLeft | Qt::AlignVCenter, text);
}
}

BtkFocusOverlay::BtkFocusOverlay(QWidget *parent, Qt::WindowFlags flags)
   : QWidget(parent, flags | Qt::FramelessWindowHint)
{
   setAttribute(Qt::WA_TransparentForMouseEvents);
   setFocusPolicy(Qt::NoFocus);
   setWindowOpacity(0.92);
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
   resize(sizeHint());
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
   const int width = 520;
   const int innerWidth = width - 28;
   const QFontMetrics fm(font());

   int height = 22;
   height += fm.height() + 12;
   height += fm.height() + 18;
   height += fm.height() + 18;

   height += fm.height() + 6;
   height += btkWrappedTextHeight(fm, innerWidth, m_snapshot.focusWidgetDescription) + 8;
   height += btkWrappedTextHeight(fm, innerWidth, QString("path=%1").arg(m_snapshot.focusWidgetPath)) + 12;

   if (! m_snapshot.ownerSummaries.isEmpty()) {
      height += fm.height() + 6;
      for (const auto &ownerSummary : m_snapshot.ownerSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, ownerSummary) + 8;
      }
      height += 4;
   }

   if (! m_snapshot.tokenSummaries.isEmpty()) {
      height += fm.height() + 6;
      for (const auto &token : m_snapshot.tokenSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, token) + 8;
      }
      height += 4;
   }

   if (m_targetWidget) {
      const QString targetContext = QApplication::btkDescribeWidgetContext(m_targetWidget);
      const QString targetPath = BtkFocusDiagnostics::describeWidgetTreePath(m_targetWidget);
      const QString targetDecision = QApplication::btkDescribeFocusDecision(m_targetWidget, Qt::OtherFocusReason);

      height += fm.height() + 6;
      height += btkWrappedTextHeight(fm, innerWidth, targetContext) + 8;
      height += btkWrappedTextHeight(fm, innerWidth, QString("path=%1").arg(targetPath)) + 8;
      height += btkWrappedTextHeight(fm, innerWidth, targetDecision) + 8;
   }

   height += fm.height() + 6;
   height += btkWrappedTextHeight(fm, innerWidth, m_snapshot.currentStateText) + 16;

   return QSize(width, qMax(220, height));
}

void BtkFocusOverlay::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing, true);

   const QRect panelRect = rect().adjusted(0, 0, -1, -1);
   painter.setPen(QPen(btkOverlayBorderColor(), 1));
   painter.setBrush(btkOverlayBackgroundColor());
   painter.drawRoundedRect(panelRect, 12, 12);

   const int left = 14;
   const int right = 14;
   const int contentWidth = width() - left - right;
   int y = 14;

   QFont titleFont = painter.font();
   titleFont.setBold(true);
   titleFont.setPointSize(titleFont.pointSize() + 1);
   painter.setFont(titleFont);
   painter.setPen(btkOverlayTitleColor());
   painter.drawText(QRect(left, y, contentWidth, painter.fontMetrics().height()), Qt::AlignLeft | Qt::AlignVCenter,
      QString("BTK Focus HUD"));
   y += painter.fontMetrics().height() + 10;

   QFont bodyFont = painter.font();
   bodyFont.setBold(false);
   painter.setFont(bodyFont);

   btkDrawChip(painter, left, y, QString("popup owner"),
      m_snapshot.activePopupOwnerId.isEmpty() ? QString("<none>") : m_snapshot.activePopupOwnerId,
      QColor(90, 220, 160, 210));

   btkDrawChip(painter, left + 170, y, QString("modal owner"),
      m_snapshot.activeModalOwnerId.isEmpty() ? QString("<none>") : m_snapshot.activeModalOwnerId,
      QColor(255, 196, 92, 210));

   btkDrawChip(painter, left + 340, y, QString("refresh"),
      QString::number(m_refreshInterval) + QString("ms"),
      QColor(120, 150, 255, 210));

   y += painter.fontMetrics().height() + 18;

   btkDrawChip(painter, left, y, QString("focus owner"),
      m_snapshot.focusOwnerId.isEmpty() ? QString("<none>") : m_snapshot.focusOwnerId,
      QColor(120, 150, 255, 210));

   btkDrawChip(painter, left + 260, y, QString("focus surface"),
      m_snapshot.focusSurfaceId.isEmpty() ? QString("<none>") : m_snapshot.focusSurfaceId,
      QColor(162, 135, 255, 210));

   y += painter.fontMetrics().height() + 18;

   btkDrawSectionHeader(painter, left, y, contentWidth, QString("Focus Widget"));
   btkDrawWrappedBlock(painter, left, y, contentWidth, m_snapshot.focusWidgetDescription, btkOverlayValueColor());
   btkDrawWrappedBlock(painter, left, y, contentWidth,
      QString("path=%1").arg(m_snapshot.focusWidgetPath.isEmpty() ? QString("<null>") : m_snapshot.focusWidgetPath),
      QColor(182, 193, 225));

   if (! m_snapshot.ownerSummaries.isEmpty()) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Owner Groups"));
      for (const auto &ownerSummary : m_snapshot.ownerSummaries) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, ownerSummary, btkOverlayAccentForLine(ownerSummary));
      }
   }

   if (! m_snapshot.tokenSummaries.isEmpty()) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Active Focus Tokens"));
      for (const auto &token : m_snapshot.tokenSummaries) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, token, btkOverlayAccentForLine(token));
      }
   }

   if (m_targetWidget) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Target Widget"));
      btkDrawWrappedBlock(painter, left, y, contentWidth,
         QApplication::btkDescribeWidgetContext(m_targetWidget), btkOverlayValueColor());
      btkDrawWrappedBlock(painter, left, y, contentWidth,
         QString("path=%1").arg(BtkFocusDiagnostics::describeWidgetTreePath(m_targetWidget)), QColor(182, 193, 225));
      btkDrawWrappedBlock(painter, left, y, contentWidth,
         QApplication::btkDescribeFocusDecision(m_targetWidget, Qt::OtherFocusReason),
         btkOverlayAccentForLine(QApplication::btkDescribeFocusDecision(m_targetWidget, Qt::OtherFocusReason)));
   }

   btkDrawSectionHeader(painter, left, y, contentWidth, QString("Current Diagnostics"));
   btkDrawWrappedBlock(painter, left, y, contentWidth,
      m_snapshot.currentStateText.isEmpty() ? QString("<none>") : m_snapshot.currentStateText,
      QColor(205, 214, 235));
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
   lines.append(QString("BTK Focus HUD"));
   lines.append(QString("activePopupOwner=%1").arg(m_snapshot.activePopupOwnerId.isEmpty() ? QString("<none>") : m_snapshot.activePopupOwnerId));
   lines.append(QString("activeModalOwner=%1").arg(m_snapshot.activeModalOwnerId.isEmpty() ? QString("<none>") : m_snapshot.activeModalOwnerId));
   lines.append(QString("focusWidget=%1").arg(m_snapshot.focusWidgetDescription));
   lines.append(QString("focusWidgetPath=%1").arg(m_snapshot.focusWidgetPath));
   lines.append(QString("focusOwner=%1").arg(m_snapshot.focusOwnerId.isEmpty() ? QString("<none>") : m_snapshot.focusOwnerId));
   lines.append(QString("focusSurface=%1").arg(m_snapshot.focusSurfaceId.isEmpty() ? QString("<none>") : m_snapshot.focusSurfaceId));

   if (! m_snapshot.ownerSummaries.isEmpty()) {
      lines.append(QString("ownerGroups:"));
      lines.append(m_snapshot.ownerSummaries);
   }

   if (! m_snapshot.tokenSummaries.isEmpty()) {
      lines.append(QString("tokens:"));
      lines.append(m_snapshot.tokenSummaries);
   }

   if (m_targetWidget) {
      lines.append(QString("targetWidget=%1").arg(BtkFocusDiagnostics::describeWidgetTreePath(m_targetWidget)));
      lines.append(QString("targetContext=%1").arg(QApplication::btkDescribeWidgetContext(m_targetWidget)));
      lines.append(QString("targetDecision=%1").arg(QApplication::btkDescribeFocusDecision(m_targetWidget, Qt::OtherFocusReason)));
   }

   lines.append(QString("currentState:"));
   lines.append(m_snapshot.currentStateText);

   return lines.join('\n');
}
