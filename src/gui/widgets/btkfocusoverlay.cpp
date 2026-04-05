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

QString btkPanelPresetToString(BtkFocusOverlay::PanelPreset preset)
{
   switch (preset) {
      case BtkFocusOverlay::PanelPreset::Compact:
         return QString("Compact");
      case BtkFocusOverlay::PanelPreset::OwnerCentric:
         return QString("OwnerCentric");
      case BtkFocusOverlay::PanelPreset::Analysis:
         return QString("Analysis");
      case BtkFocusOverlay::PanelPreset::Full:
         return QString("Full");
   }

   return QString("Unknown");
}

QColor btkOverlayAccentForLine(const QString &line)
{
   if (line.contains("decision=Reject") || (line.contains("blockingOwner=") && ! line.contains("blockingOwner=<none>"))
      || line.contains("blocker=") || line.contains("blockedRoutes=") || line.contains("sameOwner=false")) {
      return QColor(255, 110, 110);
   }

   if (line.contains("decision=Transfer") || line.contains("OwnerExclusive") || line.contains("ApplicationExclusive")
      || line.contains("SystemExclusive") || line.contains("WindowExclusive") || line.contains("focusOwnerPopupStack")) {
      return QColor(255, 196, 92);
   }

   if (line.contains("decision=Share") || line.contains("activePopupOwner=") || line.contains("activeModalOwner=")
      || (line.contains("owner=") && line.contains("tokens=")) || line.contains("sameOwner=true")) {
      return QColor(90, 220, 160);
   }

   return QColor(170, 186, 220);
}

QString btkNormalizeOverlayRelationship(const QString &label, const QString &leftName, const QString &leftValue,
   const QString &rightName, const QString &rightValue)
{
   const QString normalizedLeft = leftValue.isEmpty() ? QString("<none>") : leftValue;
   const QString normalizedRight = rightValue.isEmpty() ? QString("<none>") : rightValue;

   return QString("%1 %2=%3 %4=%5 sameOwner=%6")
      .arg(label)
      .arg(leftName)
      .arg(normalizedLeft)
      .arg(rightName)
      .arg(normalizedRight)
      .arg(normalizedLeft == normalizedRight && normalizedLeft != QString("<none>") ? QString("true") : QString("false"));
}

QString btkExtractOverlayField(const QString &line, const QString &fieldName)
{
   const QString marker = fieldName + QString("=");
   const int pos = line.indexOf(marker);

   if (pos == -1) {
      return QString();
   }

   const int valueStart = pos + marker.size();
   int valueEnd = line.indexOf(' ', valueStart);
   if (valueEnd == -1) {
      valueEnd = line.size();
   }

   return line.mid(valueStart, valueEnd - valueStart).trimmed();
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
   m_snapshot.targetDecisionSummary.clear();

   if (m_targetWidget) {
      m_snapshot.targetDecisionSummary = QApplication::btkDescribeFocusDecision(m_targetWidget, Qt::OtherFocusReason);
      if (m_snapshot.targetDecisionSummary.contains("decision=Reject")
         || (m_snapshot.targetDecisionSummary.contains("blockingOwner=") && ! m_snapshot.targetDecisionSummary.contains("blockingOwner=<none>"))) {
         const QString targetBlockedSummary = QString("target %1").arg(m_snapshot.targetDecisionSummary);
         if (! m_snapshot.blockedRouteSummaries.contains(targetBlockedSummary)) {
            m_snapshot.blockedRouteSummaries.append(targetBlockedSummary);
         }
      }
   }

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

void BtkFocusOverlay::setVisiblePanels(PanelFlags panels)
{
   if (m_visiblePanels == panels) {
      return;
   }

   m_visiblePanels = panels;
   refreshDiagnostics();
}

BtkFocusOverlay::PanelFlags BtkFocusOverlay::visiblePanels() const
{
   return m_visiblePanels;
}

void BtkFocusOverlay::setPanelPreset(PanelPreset preset)
{
   m_panelPreset = preset;

   switch (preset) {
      case PanelPreset::Compact:
         setVisiblePanels(SummaryPanel | FocusPanel | BlockedPanel);
         break;
      case PanelPreset::OwnerCentric:
         setVisiblePanels(SummaryPanel | OwnerPanel | PopupPanel | RelationshipPanel | TokenPanel | BlockedPanel);
         break;
      case PanelPreset::Analysis:
         setVisiblePanels(SummaryPanel | FocusPanel | OwnerPanel | PopupPanel | RelationshipPanel | TokenPanel | TargetPanel | BlockedPanel);
         break;
      case PanelPreset::Full:
         setVisiblePanels(AllPanels);
         break;
   }
}

BtkFocusOverlay::PanelPreset BtkFocusOverlay::panelPreset() const
{
   return m_panelPreset;
}

void BtkFocusOverlay::cyclePanelPreset()
{
   switch (m_panelPreset) {
      case PanelPreset::Compact:
         setPanelPreset(PanelPreset::OwnerCentric);
         break;
      case PanelPreset::OwnerCentric:
         setPanelPreset(PanelPreset::Analysis);
         break;
      case PanelPreset::Analysis:
         setPanelPreset(PanelPreset::Full);
         break;
      case PanelPreset::Full:
         setPanelPreset(PanelPreset::Compact);
         break;
   }
}

void BtkFocusOverlay::setBlockedRoutesOnly(bool enabled)
{
   if (m_blockedRoutesOnly == enabled) {
      return;
   }

   m_blockedRoutesOnly = enabled;
   refreshDiagnostics();
}

bool BtkFocusOverlay::blockedRoutesOnly() const
{
   return m_blockedRoutesOnly;
}

void BtkFocusOverlay::setPanelVisible(Panel panel, bool visible)
{
   PanelFlags panels = m_visiblePanels;

   if (visible) {
      panels |= panel;
   } else {
      panels &= ~PanelFlags(panel);
   }

   setVisiblePanels(panels);
}

bool BtkFocusOverlay::isPanelVisible(Panel panel) const
{
   return m_visiblePanels.testFlag(panel);
}

bool BtkFocusOverlay::shouldRenderPanel(Panel panel) const
{
   if (! isPanelVisible(panel)) {
      return false;
   }

   if (! m_blockedRoutesOnly) {
      return true;
   }

   return panel == SummaryPanel || panel == BlockedPanel || panel == TargetPanel || panel == RelationshipPanel;
}

QSize BtkFocusOverlay::sizeHint() const
{
   const int width = 560;
   const int innerWidth = width - 28;
   const QFontMetrics fm(font());

   int height = 22;
   height += fm.height() + 12;

   if (shouldRenderPanel(SummaryPanel)) {
      height += fm.height() + 18;
      height += fm.height() + 18;
      height += fm.height() + 18;
      height += fm.height() + 18;
   }

   if (shouldRenderPanel(FocusPanel)) {
      height += fm.height() + 6;
      height += btkWrappedTextHeight(fm, innerWidth, m_snapshot.focusWidgetDescription) + 8;
      height += btkWrappedTextHeight(fm, innerWidth, QString("path=%1").arg(m_snapshot.focusWidgetPath)) + 8;
      height += btkWrappedTextHeight(fm, innerWidth, m_snapshot.focusPopupRelationship) + 12;
   }

   if (shouldRenderPanel(OwnerPanel)
      && (! m_snapshot.ownerSummaries.isEmpty() || ! m_snapshot.blockerSummaries.isEmpty() || ! m_snapshot.blockerDetailSummaries.isEmpty())) {
      height += fm.height() + 6;
      for (const auto &ownerSummary : m_snapshot.ownerSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, ownerSummary) + 8;
      }
      for (const auto &blockerSummary : m_snapshot.blockerSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, blockerSummary) + 8;
      }
      for (const auto &blockerDetail : m_snapshot.blockerDetailSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, blockerDetail) + 8;
      }
      height += 4;
   }

   if (shouldRenderPanel(PopupPanel) && ! m_snapshot.popupStackSummaries.isEmpty()) {
      height += fm.height() + 6;
      for (const auto &popupSummary : m_snapshot.popupStackSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, popupSummary) + 8;
      }
      height += 4;
   }

   if (shouldRenderPanel(RelationshipPanel)
      && (! m_snapshot.comparisonClusterSummaries.isEmpty() || ! m_snapshot.relationshipSummaries.isEmpty()
         || (m_targetWidget && ! targetRelationshipDigests().isEmpty())
         || (m_targetWidget && ! targetBlockerDigests().isEmpty()) || ! mismatchDigests().isEmpty())) {
      height += fm.height() + 6;
      for (const auto &cluster : m_snapshot.comparisonClusterSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, cluster) + 8;
      }
      for (const auto &relationship : m_snapshot.relationshipSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, relationship) + 8;
      }
      for (const auto &relationship : targetRelationshipDigests()) {
         height += btkWrappedTextHeight(fm, innerWidth, relationship) + 8;
      }
      for (const auto &relationship : targetBlockerDigests()) {
         height += btkWrappedTextHeight(fm, innerWidth, relationship) + 8;
      }
      for (const auto &mismatch : mismatchDigests()) {
         height += btkWrappedTextHeight(fm, innerWidth, mismatch) + 8;
      }
      height += 4;
   }

   if (shouldRenderPanel(TokenPanel) && ! m_snapshot.tokenSummaries.isEmpty()) {
      height += fm.height() + 6;
      for (const auto &token : m_snapshot.tokenSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, token) + 8;
      }
      height += 4;
   }

   if (shouldRenderPanel(TargetPanel) && m_targetWidget) {
      const QString targetContext = QApplication::btkDescribeWidgetContext(m_targetWidget);
      const QString targetPath = BtkFocusDiagnostics::describeWidgetTreePath(m_targetWidget);
      const QString targetDecision = targetDecisionSummary();
      const QString targetRelationship = targetRelationshipSummary();

      height += fm.height() + 6;
      height += btkWrappedTextHeight(fm, innerWidth, targetContext) + 8;
      height += btkWrappedTextHeight(fm, innerWidth, QString("path=%1").arg(targetPath)) + 8;
      height += btkWrappedTextHeight(fm, innerWidth, targetRelationship) + 8;
      height += btkWrappedTextHeight(fm, innerWidth, targetDecision) + 8;
   }

   if (shouldRenderPanel(BlockedPanel)
      && (! m_snapshot.blockedReasonSummaries.isEmpty() || ! m_snapshot.blockedRouteSummaries.isEmpty())) {
      height += fm.height() + 6;
      for (const auto &reason : m_snapshot.blockedReasonSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, reason) + 8;
      }
      for (const auto &blocked : m_snapshot.blockedRouteSummaries) {
         height += btkWrappedTextHeight(fm, innerWidth, blocked) + 8;
      }
      height += 4;
   }

   if (shouldRenderPanel(RawPanel)) {
      height += fm.height() + 6;
      height += btkWrappedTextHeight(fm, innerWidth, m_snapshot.currentStateText) + 16;
   }

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
   const int contentWidth = width() - 28;
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

   if (shouldRenderPanel(SummaryPanel)) {
      btkDrawChip(painter, left, y, QString("popup owner"),
         m_snapshot.activePopupOwnerId.isEmpty() ? QString("<none>") : m_snapshot.activePopupOwnerId,
         QColor(90, 220, 160, 210));

      btkDrawChip(painter, left + 170, y, QString("modal owner"),
         m_snapshot.activeModalOwnerId.isEmpty() ? QString("<none>") : m_snapshot.activeModalOwnerId,
         QColor(255, 196, 92, 210));

      btkDrawChip(painter, left + 340, y, QString("refresh"),
         QString::number(m_refreshInterval) + QString("ms"),
         QColor(120, 150, 255, 210));

      btkDrawChip(painter, left + 440, y, QString("blocked"),
         QString::number(m_snapshot.blockedRouteCount()),
         m_snapshot.blockedRouteCount() > 0 ? QColor(255, 110, 110, 210) : QColor(90, 220, 160, 210));

      y += painter.fontMetrics().height() + 18;

      btkDrawChip(painter, left, y, QString("focus owner"),
         m_snapshot.focusOwnerId.isEmpty() ? QString("<none>") : m_snapshot.focusOwnerId,
         QColor(120, 150, 255, 210));

      btkDrawChip(painter, left + 180, y, QString("focus surface"),
         m_snapshot.focusSurfaceId.isEmpty() ? QString("<none>") : m_snapshot.focusSurfaceId,
         QColor(162, 135, 255, 210));

      btkDrawChip(painter, left + 360, y, QString("popups"),
         QString::number(m_snapshot.popupCount()),
         QColor(120, 150, 255, 210));

      btkDrawChip(painter, left + 455, y, QString("blockers"),
         QString::number(m_snapshot.blockerCount()),
         m_snapshot.blockerCount() > 0 ? QColor(255, 196, 92, 210) : QColor(90, 220, 160, 210));

      y += painter.fontMetrics().height() + 18;

      btkDrawChip(painter, left, y, QString("preset"),
         btkPanelPresetToString(m_panelPreset),
         QColor(120, 150, 255, 210));

      btkDrawChip(painter, left + 150, y, QString("relations"),
         QString::number(m_snapshot.relationshipCount()),
         m_snapshot.relationshipCount() > 0 ? QColor(162, 135, 255, 210) : QColor(90, 220, 160, 210));

      btkDrawChip(painter, left + 300, y, QString("reasons"),
         QString::number(m_snapshot.blockedReasonCount()),
         m_snapshot.blockedReasonCount() > 0 ? QColor(255, 196, 92, 210) : QColor(90, 220, 160, 210));

      y += painter.fontMetrics().height() + 18;

      btkDrawChip(painter, left, y, QString("mismatches"),
         QString::number(mismatchCount()),
         mismatchCount() > 0 ? QColor(255, 110, 110, 210) : QColor(90, 220, 160, 210));

      btkDrawChip(painter, left + 170, y, QString("clusters"),
         QString::number(m_snapshot.comparisonClusterCount()),
         m_snapshot.comparisonClusterCount() > 0 ? QColor(162, 135, 255, 210) : QColor(90, 220, 160, 210));

      if (m_blockedRoutesOnly) {
         btkDrawChip(painter, left + 320, y, QString("mode"), QString("Blocked"), QColor(255, 110, 110, 210));
      }

      y += painter.fontMetrics().height() + 18;
   }

   if (shouldRenderPanel(FocusPanel)) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Focus Widget"));
      btkDrawWrappedBlock(painter, left, y, contentWidth, m_snapshot.focusWidgetDescription, btkOverlayValueColor());
      btkDrawWrappedBlock(painter, left, y, contentWidth,
         QString("path=%1").arg(m_snapshot.focusWidgetPath.isEmpty() ? QString("<null>") : m_snapshot.focusWidgetPath),
         QColor(182, 193, 225));
      btkDrawWrappedBlock(painter, left, y, contentWidth,
         m_snapshot.focusPopupRelationship.isEmpty() ? QString("popupRelationship=<none>") : m_snapshot.focusPopupRelationship,
         btkOverlayAccentForLine(m_snapshot.focusPopupRelationship));
   }

   if (shouldRenderPanel(OwnerPanel) && ! m_snapshot.ownerSummaries.isEmpty()) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Owner Groups"));
      for (const auto &ownerSummary : m_snapshot.ownerSummaries) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, ownerSummary, btkOverlayAccentForLine(ownerSummary));
      }
   }

   if (shouldRenderPanel(OwnerPanel) && ! m_snapshot.blockerSummaries.isEmpty()) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Blocker Groups"));
      for (const auto &blockerSummary : m_snapshot.blockerSummaries) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, blockerSummary, btkOverlayAccentForLine(blockerSummary));
      }
   }

   if (shouldRenderPanel(OwnerPanel) && ! m_snapshot.blockerDetailSummaries.isEmpty()) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Blocker Drilldown"));
      for (const auto &blockerDetail : m_snapshot.blockerDetailSummaries) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, blockerDetail, btkOverlayAccentForLine(blockerDetail));
      }
   }

   if (shouldRenderPanel(PopupPanel) && ! m_snapshot.popupStackSummaries.isEmpty()) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Popup Stack"));
      for (const auto &popupSummary : m_snapshot.popupStackSummaries) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, popupSummary, QColor(182, 193, 225));
      }
   }

   if (shouldRenderPanel(RelationshipPanel)
      && (! m_snapshot.comparisonClusterSummaries.isEmpty() || ! m_snapshot.relationshipSummaries.isEmpty()
         || (m_targetWidget && ! targetRelationshipDigests().isEmpty())
         || (m_targetWidget && ! targetBlockerDigests().isEmpty()))) {
      if (! m_snapshot.comparisonClusterSummaries.isEmpty()) {
         btkDrawSectionHeader(painter, left, y, contentWidth, QString("Comparison Clusters"));
         for (const auto &cluster : m_snapshot.comparisonClusterSummaries) {
            btkDrawWrappedBlock(painter, left, y, contentWidth, cluster, btkOverlayAccentForLine(cluster));
         }
      }

      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Relationships"));
      for (const auto &relationship : m_snapshot.relationshipSummaries) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, relationship, btkOverlayAccentForLine(relationship));
      }
      for (const auto &relationship : targetRelationshipDigests()) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, relationship, btkOverlayAccentForLine(relationship));
      }
      for (const auto &relationship : targetBlockerDigests()) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, relationship, btkOverlayAccentForLine(relationship));
      }
   }

   if (shouldRenderPanel(RelationshipPanel) && ! mismatchDigests().isEmpty()) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Mismatches"));
      for (const auto &mismatch : mismatchDigests()) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, mismatch, btkOverlayAccentForLine(mismatch));
      }
   }

   if (shouldRenderPanel(TokenPanel) && ! m_snapshot.tokenSummaries.isEmpty()) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Active Focus Tokens"));
      for (const auto &token : m_snapshot.tokenSummaries) {
         btkDrawWrappedBlock(painter, left, y, contentWidth, token, btkOverlayAccentForLine(token));
      }
   }

   if (shouldRenderPanel(TargetPanel) && m_targetWidget) {
      const QString targetContext = QApplication::btkDescribeWidgetContext(m_targetWidget);
      const QString targetPath = BtkFocusDiagnostics::describeWidgetTreePath(m_targetWidget);
      const QString targetDecision = targetDecisionSummary();
      const QString targetRelationship = targetRelationshipSummary();

      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Target Widget"));
      btkDrawWrappedBlock(painter, left, y, contentWidth, targetContext, btkOverlayValueColor());
      btkDrawWrappedBlock(painter, left, y, contentWidth,
         QString("path=%1").arg(targetPath), QColor(182, 193, 225));
      btkDrawWrappedBlock(painter, left, y, contentWidth,
         targetRelationship, btkOverlayAccentForLine(targetRelationship));
      btkDrawWrappedBlock(painter, left, y, contentWidth,
         targetDecision, btkOverlayAccentForLine(targetDecision));
   }

   if (shouldRenderPanel(BlockedPanel)
      && (! m_snapshot.blockedReasonSummaries.isEmpty() || ! m_snapshot.blockedRouteSummaries.isEmpty())) {
      if (! m_snapshot.blockedReasonSummaries.isEmpty()) {
         btkDrawSectionHeader(painter, left, y, contentWidth, QString("Blocked Reason Groups"));
         for (const auto &reason : m_snapshot.blockedReasonSummaries) {
            btkDrawWrappedBlock(painter, left, y, contentWidth, reason, btkOverlayAccentForLine(reason));
         }
      }

      if (! m_snapshot.blockedRouteSummaries.isEmpty()) {
         btkDrawSectionHeader(painter, left, y, contentWidth, QString("Blocked / Exclusive Routes"));
         for (const auto &blocked : m_snapshot.blockedRouteSummaries) {
            btkDrawWrappedBlock(painter, left, y, contentWidth, blocked, btkOverlayAccentForLine(blocked));
         }
      }
   }

   if (shouldRenderPanel(RawPanel)) {
      btkDrawSectionHeader(painter, left, y, contentWidth, QString("Current Diagnostics"));
      btkDrawWrappedBlock(painter, left, y, contentWidth,
         m_snapshot.currentStateText.isEmpty() ? QString("<none>") : m_snapshot.currentStateText,
         QColor(205, 214, 235));
   }
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

QString BtkFocusOverlay::targetRelationshipSummary() const
{
   return m_targetWidget ? QApplication::btkDescribePopupRelationship(m_targetWidget) : QString();
}

QStringList BtkFocusOverlay::targetRelationshipDigests() const
{
   if (! m_targetWidget) {
      return QStringList();
   }

   const QString targetOwner = QApplication::btkOwnerId(m_targetWidget);

   return QStringList({
      btkNormalizeOverlayRelationship(QString("focusVsTarget"), QString("focusOwner"), m_snapshot.focusOwnerId,
         QString("targetOwner"), targetOwner),
      btkNormalizeOverlayRelationship(QString("targetVsPopup"), QString("targetOwner"), targetOwner,
         QString("popupOwner"), m_snapshot.activePopupOwnerId),
      btkNormalizeOverlayRelationship(QString("targetVsModal"), QString("targetOwner"), targetOwner,
         QString("modalOwner"), m_snapshot.activeModalOwnerId)
   });
}

QStringList BtkFocusOverlay::targetBlockerDigests() const
{
   if (! m_targetWidget) {
      return QStringList();
   }

   const QString targetOwner = QApplication::btkOwnerId(m_targetWidget);
   const QString targetSurface = QApplication::btkSurfaceId(m_targetWidget);
   const QString blockerOwner = btkExtractOverlayField(targetDecisionSummary(), QString("blockingOwner"));
   const QString blockerSurface = btkExtractOverlayField(targetDecisionSummary(), QString("blockingSurface"));
   const QString reason = btkExtractOverlayField(targetDecisionSummary(), QString("reason"));

   return QStringList({
      btkNormalizeOverlayRelationship(QString("targetVsBlocker"), QString("targetOwner"), targetOwner,
         QString("blockerOwner"), blockerOwner),
      btkNormalizeOverlayRelationship(QString("targetSurfaceVsBlocker"), QString("targetSurface"), targetSurface,
         QString("blockerSurface"), blockerSurface),
      QString("targetBlockReason reason=%1 blockerOwner=%2 blockerSurface=%3")
         .arg(reason.isEmpty() ? QString("<none>") : reason)
         .arg(blockerOwner.isEmpty() ? QString("<none>") : blockerOwner)
         .arg(blockerSurface.isEmpty() ? QString("<none>") : blockerSurface)
   });
}

QStringList BtkFocusOverlay::mismatchDigests() const
{
   QStringList retval;

   const auto collectMismatch = [&](const QStringList &summaries) {
      for (const auto &summary : summaries) {
         if (summary.contains("sameOwner=false")
            || (summary.contains("blockerOwner=") && ! summary.contains("blockerOwner=<none>"))
            || (summary.contains("reason=") && ! summary.contains("reason=<none>"))) {
            if (! retval.contains(summary)) {
               retval.append(summary);
            }
         }
      }
   };

   collectMismatch(m_snapshot.relationshipSummaries);
   collectMismatch(targetRelationshipDigests());
   collectMismatch(targetBlockerDigests());

   return retval;
}

int BtkFocusOverlay::mismatchCount() const
{
   return mismatchDigests().size();
}

QString BtkFocusOverlay::targetDecisionSummary() const
{
   if (! m_snapshot.targetDecisionSummary.isEmpty()) {
      return m_snapshot.targetDecisionSummary;
   }

   return m_targetWidget ? QApplication::btkDescribeFocusDecision(m_targetWidget, Qt::OtherFocusReason) : QString();
}

QString BtkFocusOverlay::buildDisplayText() const
{
   QStringList lines;
   lines.append(QString("BTK Focus HUD"));

   if (shouldRenderPanel(SummaryPanel)) {
      lines.append(QString("activePopupOwner=%1").arg(m_snapshot.activePopupOwnerId.isEmpty() ? QString("<none>") : m_snapshot.activePopupOwnerId));
      lines.append(QString("activeModalOwner=%1").arg(m_snapshot.activeModalOwnerId.isEmpty() ? QString("<none>") : m_snapshot.activeModalOwnerId));
      lines.append(QString("focusOwner=%1").arg(m_snapshot.focusOwnerId.isEmpty() ? QString("<none>") : m_snapshot.focusOwnerId));
      lines.append(QString("focusSurface=%1").arg(m_snapshot.focusSurfaceId.isEmpty() ? QString("<none>") : m_snapshot.focusSurfaceId));
      lines.append(QString("blockedCount=%1").arg(m_snapshot.blockedRouteCount()));
      lines.append(QString("popupCount=%1").arg(m_snapshot.popupCount()));
      lines.append(QString("blockerCount=%1").arg(m_snapshot.blockerCount()));
      lines.append(QString("blockedReasonCount=%1").arg(m_snapshot.blockedReasonCount()));
      lines.append(QString("relationshipCount=%1").arg(m_snapshot.relationshipCount()));
      lines.append(QString("comparisonClusterCount=%1").arg(m_snapshot.comparisonClusterCount()));
      lines.append(QString("mismatchCount=%1").arg(mismatchCount()));
      lines.append(QString("preset=%1").arg(btkPanelPresetToString(m_panelPreset)));
      lines.append(QString("blockedOnly=%1").arg(m_blockedRoutesOnly ? QString("true") : QString("false")));
   }

   if (shouldRenderPanel(FocusPanel)) {
      lines.append(QString("focusWidget=%1").arg(m_snapshot.focusWidgetDescription));
      lines.append(QString("focusWidgetPath=%1").arg(m_snapshot.focusWidgetPath));
      lines.append(QString("focusPopupRelationship=%1").arg(m_snapshot.focusPopupRelationship));
   }

   if (shouldRenderPanel(OwnerPanel) && ! m_snapshot.ownerSummaries.isEmpty()) {
      lines.append(QString("ownerGroups:"));
      lines.append(m_snapshot.ownerSummaries);
   }

   if (shouldRenderPanel(OwnerPanel) && ! m_snapshot.blockerSummaries.isEmpty()) {
      lines.append(QString("blockerGroups:"));
      lines.append(m_snapshot.blockerSummaries);
   }

   if (shouldRenderPanel(OwnerPanel) && ! m_snapshot.blockerDetailSummaries.isEmpty()) {
      lines.append(QString("blockerDrilldown:"));
      lines.append(m_snapshot.blockerDetailSummaries);
   }

   if (shouldRenderPanel(PopupPanel) && ! m_snapshot.popupStackSummaries.isEmpty()) {
      lines.append(QString("popupStack:"));
      lines.append(m_snapshot.popupStackSummaries);
   }

   if (shouldRenderPanel(RelationshipPanel)
      && (! m_snapshot.comparisonClusterSummaries.isEmpty() || ! m_snapshot.relationshipSummaries.isEmpty()
         || (m_targetWidget && ! targetRelationshipDigests().isEmpty())
         || (m_targetWidget && ! targetBlockerDigests().isEmpty()))) {
      lines.append(QString("comparisonClusters:"));
      lines.append(m_snapshot.comparisonClusterSummaries);
      lines.append(QString("relationships:"));
      lines.append(m_snapshot.relationshipSummaries);
      lines.append(targetRelationshipDigests());
      lines.append(targetBlockerDigests());
   }

   if (shouldRenderPanel(RelationshipPanel) && ! mismatchDigests().isEmpty()) {
      lines.append(QString("mismatches:"));
      lines.append(mismatchDigests());
   }

   if (shouldRenderPanel(TokenPanel) && ! m_snapshot.tokenSummaries.isEmpty()) {
      lines.append(QString("tokens:"));
      lines.append(m_snapshot.tokenSummaries);
   }

   if (shouldRenderPanel(TargetPanel) && m_targetWidget) {
      lines.append(QString("targetWidget=%1").arg(BtkFocusDiagnostics::describeWidgetTreePath(m_targetWidget)));
      lines.append(QString("targetContext=%1").arg(QApplication::btkDescribeWidgetContext(m_targetWidget)));
      lines.append(QString("targetPopupRelationship=%1").arg(targetRelationshipSummary()));
      lines.append(QString("targetDecision=%1").arg(targetDecisionSummary()));
   }

   if (shouldRenderPanel(BlockedPanel)
      && (! m_snapshot.blockedReasonSummaries.isEmpty() || ! m_snapshot.blockedRouteSummaries.isEmpty())) {
      if (! m_snapshot.blockedReasonSummaries.isEmpty()) {
         lines.append(QString("blockedReasons:"));
         lines.append(m_snapshot.blockedReasonSummaries);
      }
      if (! m_snapshot.blockedRouteSummaries.isEmpty()) {
         lines.append(QString("blockedRoutes:"));
         lines.append(m_snapshot.blockedRouteSummaries);
      }
   }

   if (shouldRenderPanel(RawPanel)) {
      lines.append(QString("currentState:"));
      lines.append(m_snapshot.currentStateText);
   }

   return lines.join('\n');
}
