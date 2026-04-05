#include <QtCore/BTKFocusToken>
#include <QtCore/QByteArray>
#include <QtGui/BTKFocusOverlay>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtNetwork/QHostAddress>
#include <QtSvg/QSvgRenderer>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   const QHostAddress address(QString("127.0.0.1"));
   const QByteArray svg("<svg xmlns='http://www.w3.org/2000/svg' width='18' height='12'>"
      "<rect width='18' height='12' fill='#114488'/></svg>");
   QSvgRenderer renderer(svg);

   QWidget primary;
   primary.setObjectName("integrated-primary");
   QApplication::setBtkOwnerContext(&primary, "owner-integrated-primary", "surface-integrated-primary");

   QWidget target;
   target.setObjectName("integrated-target");
   QApplication::setBtkOwnerContext(&target, "owner-integrated-secondary", "surface-integrated-secondary");

   BtkFocusToken token("integrated-exclusive-token", "owner-integrated-primary", "surface-integrated-primary");
   token.setPriority(101);
   token.setModalityPolicy(BtkFocusToken::ModalityPolicy::OwnerExclusive);
   QApplication::setBtkFocusTokens({token});

   BtkFocusOverlay overlay;
   overlay.setTargetWidget(&target);
   overlay.setAutoRefreshEnabled(false);
   overlay.setPanelPreset(BtkFocusOverlay::PanelPreset::Analysis);
   overlay.refreshDiagnostics();

   const QString targetDecision = overlay.targetDecisionSummary();
   const QSize svgSize = renderer.defaultSize();

   return address.isNull()
      || ! address.isLoopback()
      || ! renderer.isValid()
      || svgSize.width() != 18
      || svgSize.height() != 12
      || overlay.diagnosticsText().isEmpty()
      || overlay.snapshot().comparisonClusterSummaries.isEmpty()
      || overlay.targetComparisonClusters().isEmpty()
      || overlay.targetRelationshipDigests().isEmpty()
      || overlay.targetBlockerDigests().isEmpty()
      || overlay.mismatchDigests().isEmpty()
      || ! targetDecision.contains("decision=Reject")
      || ! targetDecision.contains("blockingOwner=owner-integrated-primary") ? 1 : 0;
}
