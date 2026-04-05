#include <QtCore/BTKFocusToken>
#include <QtCore/QByteArray>
#include <QtGui/BTKFocusOverlay>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QMediaTimeRange>
#include <QtNetwork/QHostAddress>
#include <QtOpenGL/qgl.h>
#include <QtSql/QSqlDatabase>
#include <QtSvg/QSvgRenderer>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   const QHostAddress address(QString("127.0.0.1"));

   QGLFormat glFormat;
   glFormat.setDepthBufferSize(24);
   glFormat.setVersion(2, 1);

   const QByteArray svg("<svg xmlns='http://www.w3.org/2000/svg' width='20' height='14'>"
      "<rect width='20' height='14' fill='#225588'/></svg>");
   QSvgRenderer renderer(svg);

   QSqlDatabase db;

   QAudioFormat audioFormat;
   audioFormat.setSampleRate(44100);
   audioFormat.setChannelCount(2);
   audioFormat.setSampleSize(16);

   QMediaTimeRange mediaRange;
   mediaRange.addInterval(5, 25);

   QWidget primary;
   primary.setObjectName("platform-primary");
   QApplication::setBtkOwnerContext(&primary, "owner-platform-primary", "surface-platform-primary");

   QWidget target;
   target.setObjectName("platform-target");
   QApplication::setBtkOwnerContext(&target, "owner-platform-secondary", "surface-platform-secondary");

   BtkFocusToken token("platform-exclusive-token", "owner-platform-primary", "surface-platform-primary");
   token.setPriority(123);
   token.setModalityPolicy(BtkFocusToken::ModalityPolicy::OwnerExclusive);
   QApplication::setBtkFocusTokens({token});

   BtkFocusOverlay overlay;
   overlay.setTargetWidget(&target);
   overlay.setAutoRefreshEnabled(false);
   overlay.setPanelPreset(BtkFocusOverlay::PanelPreset::Analysis);
   overlay.refreshDiagnostics();

   const QSize svgSize = renderer.defaultSize();
   const QString targetDecision = overlay.targetDecisionSummary();

   return address.isNull()
      || ! address.isLoopback()
      || glFormat.depthBufferSize() != 24
      || glFormat.majorVersion() != 2
      || glFormat.minorVersion() != 1
      || ! renderer.isValid()
      || svgSize.width() != 20
      || svgSize.height() != 14
      || db.isValid()
      || db.isOpen()
      || ! audioFormat.isValid()
      || audioFormat.sampleRate() != 44100
      || audioFormat.channelCount() != 2
      || audioFormat.sampleSize() != 16
      || mediaRange.isEmpty()
      || mediaRange.earliestTime() != 5
      || mediaRange.latestTime() != 25
      || overlay.diagnosticsText().isEmpty()
      || overlay.snapshot().comparisonClusterSummaries.isEmpty()
      || overlay.targetComparisonClusters().isEmpty()
      || overlay.targetRelationshipDigests().isEmpty()
      || overlay.targetBlockerDigests().isEmpty()
      || overlay.mismatchDigests().isEmpty()
      || ! targetDecision.contains("decision=Reject")
      || ! targetDecision.contains("blockingOwner=owner-platform-primary") ? 1 : 0;
}
