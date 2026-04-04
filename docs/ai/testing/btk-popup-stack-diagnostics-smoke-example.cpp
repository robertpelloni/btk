#include <QApplication>
#include <QWidget>

int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QWidget popup;
   popup.setObjectName("popup-stack-probe");
   QApplication::setBtkOwnerContext(&popup, "owner-popup", "surface-popup");

   const QStringList popupStack = QApplication::btkPopupStackDiagnostics();
   return popupStack.size() < 0 ? 1 : 0;
}
