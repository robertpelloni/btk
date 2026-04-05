#include <QtCore/BTKFocusToken>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QWidget>

#include <iostream>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QWidget popupA1;
   popupA1.setObjectName("popup-a-1");
   popupA1.setWindowFlags(Qt::Popup);
   QApplication::setBtkOwnerContext(&popupA1, "owner-a", "surface-a-1");

   QWidget popupA2;
   popupA2.setObjectName("popup-a-2");
   popupA2.setWindowFlags(Qt::Popup);
   QApplication::setBtkOwnerContext(&popupA2, "owner-a", "surface-a-2");

   QWidget foreignTarget;
   foreignTarget.setObjectName("popup-foreign-target");
   QApplication::setBtkOwnerContext(&foreignTarget, "owner-b", "surface-target-b");

   BtkFocusToken token("popup-stack-owner-exclusive", "owner-a", "surface-a-2");
   token.setPriority(101);
   token.setModalityPolicy(BtkFocusToken::ModalityPolicy::OwnerExclusive);
   QApplication::setBtkFocusTokens({token});

   QTimer::singleShot(0, [&]() {
      popupA1.show();
      popupA2.show();

      QTimer::singleShot(0, [&]() {
         const QWidget *activePopup = QApplication::activePopupWidget();
         const QString activeOwner = QApplication::btkActivePopupOwnerId();
         const QStringList stack = QApplication::btkPopupStackDiagnostics();
         const QStringList ownerAStack = QApplication::btkPopupStackDiagnostics("owner-a");
         const QString popupRelationship = QApplication::btkDescribePopupRelationship(&foreignTarget);
         const QString targetDecision = QApplication::btkDescribeFocusDecision(&foreignTarget, Qt::PopupFocusReason);

         const bool ok = activeOwner == QString("owner-a")
            && activePopup != nullptr
            && activePopup->objectName() == QString("popup-a-2")
            && stack.size() == 2
            && stack.value(0).contains("object=popup-a-1")
            && stack.value(1).contains("object=popup-a-2")
            && ownerAStack.size() == 2
            && popupRelationship.contains("popupOwner=owner-a")
            && popupRelationship.contains("widgetOwner=owner-b")
            && popupRelationship.contains("popupAllowed=false")
            && targetDecision.contains("reason=PopupFocusReason")
            && targetDecision.contains("decision=Reject")
            && targetDecision.contains("blockingOwner=owner-a");

         if (! ok) {
            std::cout << "activeOwner=" << activeOwner.constData() << "\n"
               << "activePopup=" << (activePopup ? activePopup->objectName().constData() : "<null>") << "\n"
               << "stack=" << stack.join(QString(" || ")).constData() << "\n"
               << "ownerAStack=" << ownerAStack.join(QString(" || ")).constData() << "\n"
               << "popupRelationship=" << popupRelationship.constData() << "\n"
               << "targetDecision=" << targetDecision.constData() << std::endl;
         }

         app.exit(ok ? 0 : 1);
      });
   });

   return app.exec();
}
