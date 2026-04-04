#include <btkfocusdiagnostics.h>

#include <qapplication.h>
#include <qwidget.h>

bool BtkFocusDiagnosticsSnapshot::isEmpty() const
{
   return activePopupOwnerId.isEmpty() && activeModalOwnerId.isEmpty()
      && focusWidgetDescription.isEmpty() && tokenSummaries.isEmpty() && lines.isEmpty();
}

QString BtkFocusDiagnosticsSnapshot::toString() const
{
   return lines.join("\n");
}

BtkFocusDiagnosticsSnapshot BtkFocusDiagnostics::snapshot()
{
   BtkFocusDiagnosticsSnapshot retval;

   retval.activePopupOwnerId = QApplication::btkActivePopupOwnerId();
   retval.activeModalOwnerId = QApplication::btkActiveModalOwnerId();
   retval.focusWidgetDescription = QApplication::btkDescribeWidgetContext(QApplication::focusWidget());
   retval.lines = QApplication::btkFocusDiagnostics();

   for (const QString &line : retval.lines) {
      if (line.startsWith("token=")) {
         retval.tokenSummaries.append(line);
      }
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

   QStringList lines = snap.lines;
   lines.append(QString("focusWidgetPath=%1").arg(describeWidgetTreePath(QApplication::focusWidget())));

   return lines.join("\n");
}
