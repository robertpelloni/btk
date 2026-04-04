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
   overlay.setPanelPreset(BtkFocusOverlay::PanelPreset::Analysis);
   overlay.setPanelVisible(BtkFocusOverlay::BlockedPanel, true);
   overlay.refreshDiagnostics();

   return overlay.diagnosticsText().isEmpty()
      || overlay.snapshot().ownerSummaries.isEmpty()
      || overlay.panelPreset() != BtkFocusOverlay::PanelPreset::Analysis
      || ! overlay.isPanelVisible(BtkFocusOverlay::BlockedPanel) ? 1 : 0;
}
