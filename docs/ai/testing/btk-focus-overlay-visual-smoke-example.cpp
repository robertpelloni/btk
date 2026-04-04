#include <QApplication>
#include <QWidget>

#include <QtCore/BTKFocusToken>
#include <QtGui/BTKFocusOverlay>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QWidget widget;
   widget.setObjectName("overlay-visual-target");
   QApplication::setBtkOwnerContext(&widget, "owner-visual", "surface-visual");

   BtkFocusToken token("overlay-token", "owner-visual", "surface-visual");
   token.setPriority(25);
   QApplication::setBtkFocusTokens({token});

   BtkFocusOverlay overlay;
   overlay.setTargetWidget(&widget);
   overlay.setAutoRefreshEnabled(false);
   overlay.refreshDiagnostics();

   return overlay.diagnosticsText().isEmpty() || overlay.snapshot().ownerSummaries.isEmpty() ? 1 : 0;
}
