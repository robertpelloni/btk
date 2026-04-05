#include <QtCore/BTKFocusToken>
#include <QtGui/BTKFocusOverlay>
#include <QtGui/QApplication>
#include <QtGui/QWidget>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QWidget primary;
   primary.setObjectName("runtime-primary");
   QApplication::setBtkOwnerContext(&primary, "owner-primary", "surface-primary");

   QWidget blockedTarget;
   blockedTarget.setObjectName("runtime-blocked-target");
   QApplication::setBtkOwnerContext(&blockedTarget, "owner-secondary", "surface-secondary");

   BtkFocusToken token("runtime-exclusive-token", "owner-primary", "surface-primary");
   token.setPriority(99);
   token.setModalityPolicy(BtkFocusToken::ModalityPolicy::OwnerExclusive);
   QApplication::setBtkFocusTokens({token});

   BtkFocusOverlay overlay;
   overlay.setTargetWidget(&blockedTarget);
   overlay.setAutoRefreshEnabled(false);
   overlay.setPanelPreset(BtkFocusOverlay::PanelPreset::Analysis);
   overlay.setBlockedRoutesOnly(true);
   overlay.refreshDiagnostics();

   const QString targetDecision = overlay.targetDecisionSummary();

   return overlay.diagnosticsText().isEmpty()
      || overlay.snapshot().comparisonClusterSummaries.isEmpty()
      || overlay.targetComparisonClusters().isEmpty()
      || overlay.targetRelationshipDigests().isEmpty()
      || overlay.targetBlockerDigests().isEmpty()
      || overlay.mismatchDigests().isEmpty()
      || ! targetDecision.contains("decision=Reject")
      || ! targetDecision.contains("blockingOwner=owner-primary") ? 1 : 0;
}
