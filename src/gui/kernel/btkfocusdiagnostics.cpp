#include <btkfocusdiagnostics.h>

#include <qapplication.h>
#include <qhash.h>
#include <qwidget.h>

#include <algorithm>

namespace {
QString btkExtractField(const QString &line, const QString &fieldName)
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

QString btkNormalizeBlockedRouteSummary(const QString &line)
{
   const QString reason = btkExtractField(line, QString("reason"));
   const QString decision = btkExtractField(line, QString("decision"));
   const QString resolvedOwner = btkExtractField(line, QString("resolvedOwner"));
   const QString blockingOwner = btkExtractField(line, QString("blockingOwner"));
   const QString blockingSurface = btkExtractField(line, QString("blockingSurface"));

   if (reason.isEmpty() && decision.isEmpty() && blockingOwner.isEmpty()) {
      return line;
   }

   return QString("reason=%1 decision=%2 targetOwner=%3 blocker=%4 blockerSurface=%5")
      .arg(reason.isEmpty() ? QString("<none>") : reason)
      .arg(decision.isEmpty() ? QString("<none>") : decision)
      .arg(resolvedOwner.isEmpty() ? QString("<none>") : resolvedOwner)
      .arg(blockingOwner.isEmpty() ? QString("<none>") : blockingOwner)
      .arg(blockingSurface.isEmpty() ? QString("<none>") : blockingSurface);
}

QString btkNormalizeRelationshipSummary(const QString &label, const QString &leftName, const QString &leftValue,
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

void btkSortSummaryList(QStringList &summaries, const QString &countField)
{
   std::sort(summaries.begin(), summaries.end(), [&](const QString &left, const QString &right) {
      const int leftCount = btkExtractField(left, countField).toInt();
      const int rightCount = btkExtractField(right, countField).toInt();

      if (leftCount != rightCount) {
         return leftCount > rightCount;
      }

      return left < right;
   });
}

int btkDistinctOwnerCount(const QStringList &owners)
{
   QStringList uniqueOwners;

   for (const auto &owner : owners) {
      if (owner.isEmpty() || owner == QString("<none>") || uniqueOwners.contains(owner)) {
         continue;
      }
      uniqueOwners.append(owner);
   }

   return uniqueOwners.size();
}
}

bool BtkFocusDiagnosticsSnapshot::isEmpty() const
{
   return activePopupOwnerId.isEmpty() && activeModalOwnerId.isEmpty()
      && focusWidgetDescription.isEmpty() && focusWidgetPath.isEmpty()
      && focusPopupRelationship.isEmpty()
      && focusOwnerId.isEmpty() && focusSurfaceId.isEmpty()
      && currentStateText.isEmpty() && popupStackSummaries.isEmpty()
      && ownerSummaries.isEmpty() && tokenSummaries.isEmpty()
      && blockedRouteSummaries.isEmpty() && blockedReasonSummaries.isEmpty()
      && blockerSummaries.isEmpty() && blockerDetailSummaries.isEmpty()
      && comparisonClusterSummaries.isEmpty() && relationshipSummaries.isEmpty()
      && lines.isEmpty();
}

QString BtkFocusDiagnosticsSnapshot::toString() const
{
   return lines.join("\n");
}

BtkFocusDiagnosticsSnapshot BtkFocusDiagnostics::snapshot()
{
   BtkFocusDiagnosticsSnapshot retval;
   QWidget *focusWidget = QApplication::focusWidget();

   retval.activePopupOwnerId = QApplication::btkActivePopupOwnerId();
   retval.activeModalOwnerId = QApplication::btkActiveModalOwnerId();
   retval.focusWidgetDescription = QApplication::btkDescribeWidgetContext(focusWidget);
   retval.focusWidgetPath = describeWidgetTreePath(focusWidget);
   retval.focusPopupRelationship = QApplication::btkDescribePopupRelationship(focusWidget);
   retval.focusOwnerId = QApplication::btkOwnerId(focusWidget);
   retval.focusSurfaceId = QApplication::btkSurfaceId(focusWidget);
   retval.popupStackSummaries = QApplication::btkPopupStackDiagnostics();
   retval.lines = QApplication::btkFocusDiagnostics();

   QHash<QString, int> ownerCounts;
   QHash<QString, int> blockerCounts;
   QHash<QString, int> blockedReasonCounts;
   QHash<QString, int> blockerDetailCounts;

   for (const QString &line : retval.lines) {
      if (line.startsWith("token=")) {
         retval.tokenSummaries.append(line);

         const QString ownerMarker = QString(" owner=");
         const int ownerPos = line.indexOf(ownerMarker);
         if (ownerPos != -1) {
            const int ownerValueStart = ownerPos + ownerMarker.size();
            int ownerValueEnd = line.indexOf(' ', ownerValueStart);
            if (ownerValueEnd == -1) {
               ownerValueEnd = line.size();
            }

            const QString ownerId = line.mid(ownerValueStart, ownerValueEnd - ownerValueStart).trimmed();
            if (! ownerId.isEmpty()) {
               ownerCounts[ownerId] += 1;
            }
         }
      }

      if (line.contains("decision=Reject") || (line.contains("blockingOwner=") && ! line.contains("blockingOwner=<none>"))) {
         const QString normalized = btkNormalizeBlockedRouteSummary(line);
         retval.blockedRouteSummaries.append(normalized);

         const QString blocker = btkExtractField(normalized, QString("blocker"));
         if (! blocker.isEmpty() && blocker != QString("<none>")) {
            blockerCounts[blocker] += 1;
         }

         const QString reason = btkExtractField(normalized, QString("reason"));
         if (! reason.isEmpty() && reason != QString("<none>")) {
            blockedReasonCounts[reason] += 1;
         }

         if (! blocker.isEmpty() && blocker != QString("<none>")) {
            const QString blockerDetailKey = blocker + QString("|") + (reason.isEmpty() ? QString("<none>") : reason);
            blockerDetailCounts[blockerDetailKey] += 1;
         }
      }
   }

   for (auto iter = ownerCounts.cbegin(); iter != ownerCounts.cend(); ++iter) {
      retval.ownerSummaries.append(QString("owner=%1 tokens=%2").arg(iter.key()).arg(iter.value()));
   }

   for (auto iter = blockerCounts.cbegin(); iter != blockerCounts.cend(); ++iter) {
      retval.blockerSummaries.append(QString("blocker=%1 blockedRoutes=%2").arg(iter.key()).arg(iter.value()));
   }

   for (auto iter = blockedReasonCounts.cbegin(); iter != blockedReasonCounts.cend(); ++iter) {
      retval.blockedReasonSummaries.append(QString("blockedReason=%1 blockedRoutes=%2").arg(iter.key()).arg(iter.value()));
   }

   for (auto iter = blockerDetailCounts.cbegin(); iter != blockerDetailCounts.cend(); ++iter) {
      const QStringList parts = iter.key().split('|');
      const QString blocker = parts.value(0);
      const QString reason = parts.value(1, QString("<none>"));
      retval.blockerDetailSummaries.append(QString("blocker=%1 reason=%2 blockedRoutes=%3")
         .arg(blocker)
         .arg(reason)
         .arg(iter.value()));
   }

   btkSortSummaryList(retval.ownerSummaries, QString("tokens"));
   btkSortSummaryList(retval.blockerSummaries, QString("blockedRoutes"));
   btkSortSummaryList(retval.blockedReasonSummaries, QString("blockedRoutes"));
   btkSortSummaryList(retval.blockerDetailSummaries, QString("blockedRoutes"));

   const QString normalizedFocusOwner = retval.focusOwnerId.isEmpty() ? QString("<none>") : retval.focusOwnerId;
   const QString normalizedPopupOwner = retval.activePopupOwnerId.isEmpty() ? QString("<none>") : retval.activePopupOwnerId;
   const QString normalizedModalOwner = retval.activeModalOwnerId.isEmpty() ? QString("<none>") : retval.activeModalOwnerId;
   const int uniqueOwnerCount = btkDistinctOwnerCount({normalizedFocusOwner, normalizedPopupOwner, normalizedModalOwner});

   retval.comparisonClusterSummaries.append(QString("ownerTopology focusOwner=%1 popupOwner=%2 modalOwner=%3 uniqueOwners=%4")
      .arg(normalizedFocusOwner)
      .arg(normalizedPopupOwner)
      .arg(normalizedModalOwner)
      .arg(uniqueOwnerCount));
   retval.comparisonClusterSummaries.append(QString("ownerTopologyState focusVsPopup=%1 popupVsModal=%2 focusVsModal=%3")
      .arg(normalizedFocusOwner == normalizedPopupOwner && normalizedFocusOwner != QString("<none>") ? QString("aligned") : QString("split"))
      .arg(normalizedPopupOwner == normalizedModalOwner && normalizedPopupOwner != QString("<none>") ? QString("aligned") : QString("split"))
      .arg(normalizedFocusOwner == normalizedModalOwner && normalizedFocusOwner != QString("<none>") ? QString("aligned") : QString("split")));

   retval.relationshipSummaries.append(btkNormalizeRelationshipSummary(QString("focusVsPopup"),
      QString("focusOwner"), retval.focusOwnerId, QString("popupOwner"), retval.activePopupOwnerId));
   retval.relationshipSummaries.append(btkNormalizeRelationshipSummary(QString("popupVsModal"),
      QString("popupOwner"), retval.activePopupOwnerId, QString("modalOwner"), retval.activeModalOwnerId));

   if (! retval.focusOwnerId.isEmpty()) {
      const QStringList focusOwnerPopups = QApplication::btkPopupStackDiagnostics(retval.focusOwnerId);
      if (! focusOwnerPopups.isEmpty()) {
         retval.popupStackSummaries.append(QString("focusOwnerPopupStack:"));
         retval.popupStackSummaries.append(focusOwnerPopups);
         retval.relationshipSummaries.append(QString("focusOwnerPopupStack owner=%1 popups=%2")
            .arg(retval.focusOwnerId)
            .arg(focusOwnerPopups.size()));
      }
   }

   retval.currentStateText = retval.toString();
   if (! retval.focusWidgetPath.isEmpty()) {
      retval.currentStateText += QString("\nfocusWidgetPath=%1").arg(retval.focusWidgetPath);
   }
   if (! retval.focusPopupRelationship.isEmpty()) {
      retval.currentStateText += QString("\nfocusPopupRelationship=%1").arg(retval.focusPopupRelationship);
   }

   return retval;
}

QString BtkFocusDiagnostics::describeWidgetTreePath(const QWidget *widget)
{
   if (widget == nullptr) {
      return QString("<null>");
   }

   QStringList chain;
   const QWidget *current = widget;

   while (current) {
      const QString objectName = current->objectName().isEmpty()
         ? QString("<unnamed>")
         : current->objectName();

      chain.prepend(QString::fromLatin1(current->metaObject()->className()) + QString("(") + objectName + QString(")"));

      if (current == current->window()) {
         break;
      }

      current = current->parentWidget();
   }

   return chain.join(QStringLiteral(" -> "));
}

QString BtkFocusDiagnostics::describeCurrentState()
{
   BtkFocusDiagnosticsSnapshot snap = snapshot();

   if (! snap.currentStateText.isEmpty()) {
      return snap.currentStateText;
   }

   QStringList lines = snap.lines;
   lines.append(QString("focusWidgetPath=%1").arg(describeWidgetTreePath(QApplication::focusWidget())));

   return lines.join("\n");
}
