#include <QtCore/BTKFocusToken>
#include <QtGui/BTKFocusOverlay>
#include <QtGui/QApplication>
#include <QtGui/QWidget>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QWidget widget;
   widget.setObjectName("package-gui-target");
   QApplication::setBtkOwnerContext(&widget, "owner-gui", "surface-gui");

   BtkFocusToken token("package-gui-token", "owner-gui", "surface-gui");
   token.setPriority(17);
   QApplication::setBtkFocusTokens({token});

   BtkFocusOverlay overlay;
   overlay.setTargetWidget(&widget);
   overlay.setAutoRefreshEnabled(false);
   overlay.setPanelPreset(BtkFocusOverlay::PanelPreset::Analysis);
   overlay.refreshDiagnostics();

   return overlay.diagnosticsText().isEmpty()
      || overlay.snapshot().relationshipSummaries.isEmpty()
      || overlay.targetComparisonClusters().isEmpty()
      || overlay.targetRelationshipDigests().isEmpty()
      || overlay.targetDecisionSummary().isEmpty() ? 1 : 0;
}
