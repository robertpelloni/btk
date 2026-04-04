#include <btkfocusdiagnostics.h>

#include <qapplication.h>
#include <qhash.h>
#include <qwidget.h>

bool BtkFocusDiagnosticsSnapshot::isEmpty() const
{
   return activePopupOwnerId.isEmpty() && activeModalOwnerId.isEmpty()
      && focusWidgetDescription.isEmpty() && focusWidgetPath.isEmpty()
      && focusOwnerId.isEmpty() && focusSurfaceId.isEmpty()
      && currentStateText.isEmpty() && ownerSummaries.isEmpty()
      && tokenSummaries.isEmpty() && lines.isEmpty();
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
   retval.focusOwnerId = QApplication::btkOwnerId(focusWidget);
   retval.focusSurfaceId = QApplication::btkSurfaceId(focusWidget);
   retval.lines = QApplication::btkFocusDiagnostics();

   QHash<QString, int> ownerCounts;

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
   }

   for (auto iter = ownerCounts.cbegin(); iter != ownerCounts.cend(); ++iter) {
      retval.ownerSummaries.append(QString("owner=%1 tokens=%2").arg(iter.key()).arg(iter.value()));
   }

   retval.currentStateText = retval.toString();
   if (! retval.focusWidgetPath.isEmpty()) {
      retval.currentStateText += QString("\nfocusWidgetPath=%1").arg(retval.focusWidgetPath);
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
