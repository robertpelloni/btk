#include <QtCore/BTKFocusToken>
#include <QtGui/BTKFocusOverlay>
#include <QtGui/QApplication>
#include <QtGui/QWidget>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QWidget primary;
   primary.setObjectName("behavioral-primary");
   QApplication::setBtkOwnerContext(&primary, "owner-behavioral-primary", "surface-behavioral-primary");

   QWidget sameOwnerTarget;
   sameOwnerTarget.setObjectName("behavioral-same-owner-target");
   QApplication::setBtkOwnerContext(&sameOwnerTarget, "owner-behavioral-primary", "surface-behavioral-secondary");

   QWidget crossOwnerTarget;
   crossOwnerTarget.setObjectName("behavioral-cross-owner-target");
   QApplication::setBtkOwnerContext(&crossOwnerTarget, "owner-behavioral-secondary", "surface-behavioral-foreign");

   BtkFocusToken token("behavioral-owner-exclusive", "owner-behavioral-primary", "surface-behavioral-primary");
   token.setPriority(111);
   token.setModalityPolicy(BtkFocusToken::ModalityPolicy::OwnerExclusive);
   QApplication::setBtkFocusTokens({token});

   BtkFocusOverlay overlay;
   overlay.setAutoRefreshEnabled(false);
   overlay.setPanelPreset(BtkFocusOverlay::PanelPreset::Analysis);

   overlay.setTargetWidget(&sameOwnerTarget);
   overlay.refreshDiagnostics();

   const QString sameOwnerDecision = overlay.targetDecisionSummary();
   const QStringList sameOwnerRelationships = overlay.targetRelationshipDigests();

   overlay.setTargetWidget(&crossOwnerTarget);
   overlay.refreshDiagnostics();

   const QString crossOwnerDecision = overlay.targetDecisionSummary();
   const QStringList crossOwnerRelationships = overlay.targetRelationshipDigests();
   const QStringList crossOwnerBlockers = overlay.targetBlockerDigests();
   const QStringList mismatches = overlay.mismatchDigests();

   return overlay.diagnosticsText().isEmpty()
      || sameOwnerDecision.contains("decision=Reject")
      || ! sameOwnerDecision.contains("decision=Transfer")
      || sameOwnerRelationships.isEmpty()
      || crossOwnerDecision.isEmpty()
      || ! crossOwnerDecision.contains("decision=Reject")
      || ! crossOwnerDecision.contains("blockingOwner=owner-behavioral-primary")
      || crossOwnerRelationships.isEmpty()
      || crossOwnerBlockers.isEmpty()
      || mismatches.isEmpty() ? 1 : 0;
}
