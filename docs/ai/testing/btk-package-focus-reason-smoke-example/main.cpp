#include <QtCore/BTKFocusToken>
#include <QtGui/QApplication>
#include <QtGui/QWidget>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QWidget primary;
   primary.setObjectName("reason-primary");
   QApplication::setBtkOwnerContext(&primary, "owner-reason-primary", "surface-reason-primary");

   QWidget sameOwnerTarget;
   sameOwnerTarget.setObjectName("reason-same-owner-target");
   QApplication::setBtkOwnerContext(&sameOwnerTarget, "owner-reason-primary", "surface-reason-secondary");

   QWidget crossOwnerTarget;
   crossOwnerTarget.setObjectName("reason-cross-owner-target");
   QApplication::setBtkOwnerContext(&crossOwnerTarget, "owner-reason-secondary", "surface-reason-foreign");

   BtkFocusToken token("reason-owner-exclusive", "owner-reason-primary", "surface-reason-primary");
   token.setPriority(88);
   token.setModalityPolicy(BtkFocusToken::ModalityPolicy::OwnerExclusive);
   QApplication::setBtkFocusTokens({token});

   const QString popupReasonDecision = QApplication::btkDescribeFocusDecision(&crossOwnerTarget, Qt::PopupFocusReason);
   const QString activeWindowDecision = QApplication::btkDescribeFocusDecision(&crossOwnerTarget, Qt::ActiveWindowFocusReason);
   const QString sameOwnerPopupDecision = QApplication::btkDescribeFocusDecision(&sameOwnerTarget, Qt::PopupFocusReason);
   const QString sameOwnerWindowDecision = QApplication::btkDescribeFocusDecision(&sameOwnerTarget, Qt::ActiveWindowFocusReason);

   return ! popupReasonDecision.contains("reason=PopupFocusReason")
      || ! popupReasonDecision.contains("decision=Reject")
      || ! popupReasonDecision.contains("blockingOwner=owner-reason-primary")
      || ! activeWindowDecision.contains("reason=ActiveWindowFocusReason")
      || ! activeWindowDecision.contains("decision=Reject")
      || ! sameOwnerPopupDecision.contains("decision=Transfer")
      || ! sameOwnerWindowDecision.contains("decision=Transfer")
      || QApplication::btkWouldBlockFocusChange(&sameOwnerTarget, Qt::PopupFocusReason)
      || ! QApplication::btkWouldBlockFocusChange(&crossOwnerTarget, Qt::PopupFocusReason) ? 1 : 0;
}
