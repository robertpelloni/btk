#include <QApplication>
#include <QWidget>

#include <QtGui/BTKFocusOverlay>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QWidget widget;
   widget.setObjectName("overlay-visual-target");
   QApplication::setBtkOwnerContext(&widget, "owner-visual", "surface-visual");

   BtkFocusOverlay overlay;
   overlay.setTargetWidget(&widget);
   overlay.setAutoRefreshEnabled(false);
   overlay.refreshDiagnostics();

   return overlay.diagnosticsText().isEmpty() ? 1 : 0;
}
