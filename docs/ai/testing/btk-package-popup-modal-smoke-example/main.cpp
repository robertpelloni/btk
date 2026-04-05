#include <QtCore/BTKFocusToken>
#include <QtCore/QTimer>
#include <QtGui/BTKFocusOverlay>
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QWidget>

#include <iostream>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QWidget popup;
   popup.setObjectName("popup-owner-a");
   popup.setWindowFlags(Qt::Popup);
   QApplication::setBtkOwnerContext(&popup, "owner-popup", "surface-popup");

   QDialog modal;
   modal.setObjectName("modal-owner-b");
   modal.setWindowModality(Qt::ApplicationModal);
   QApplication::setBtkOwnerContext(&modal, "owner-modal", "surface-modal");

   QWidget target;
   target.setObjectName("popup-modal-target");
   QApplication::setBtkOwnerContext(&target, "owner-modal", "surface-target");

   BtkFocusToken token("popup-modal-exclusive", "owner-popup", "surface-popup");
   token.setPriority(77);
   token.setModalityPolicy(BtkFocusToken::ModalityPolicy::OwnerExclusive);
   QApplication::setBtkFocusTokens({token});

   QTimer::singleShot(0, [&]() {
      modal.open();
      popup.show();

      QTimer::singleShot(0, [&]() {
         BtkFocusOverlay overlay;
         overlay.setTargetWidget(&target);
         overlay.setAutoRefreshEnabled(false);
         overlay.setPanelPreset(BtkFocusOverlay::PanelPreset::Analysis);
         overlay.refreshDiagnostics();

         const QString popupOwner = QApplication::btkActivePopupOwnerId();
         const QString modalOwner = QApplication::btkActiveModalOwnerId();
         const QString popupRelationship = QApplication::btkDescribePopupRelationship(&target);
         const QString targetDecision = overlay.targetDecisionSummary();
         const QStringList targetClusters = overlay.targetComparisonClusters();
         const QStringList mismatches = overlay.mismatchDigests();

         const bool ok = popupOwner == QString("owner-popup")
            && modalOwner == QString("owner-modal")
            && popupRelationship.contains("popupOwner=owner-popup")
            && popupRelationship.contains("widgetOwner=owner-modal")
            && popupRelationship.contains("popupAllowed=false")
            && targetDecision.contains("decision=Reject")
            && targetDecision.contains("blockingOwner=owner-popup")
            && ! targetClusters.isEmpty()
            && ! mismatches.isEmpty();

         if (! ok) {
            std::cout << "popupOwner=" << popupOwner.constData() << "\n"
               << "modalOwner=" << modalOwner.constData() << "\n"
               << "popupRelationship=" << popupRelationship.constData() << "\n"
               << "targetDecision=" << targetDecision.constData() << "\n"
               << "targetClusters=" << targetClusters.join(QString(" || ")).constData() << "\n"
               << "mismatches=" << mismatches.join(QString(" || ")).constData() << std::endl;
         }

         popup.close();
         modal.close();
         app.exit(ok ? 0 : 1);
      });
   });

   return app.exec();
}
