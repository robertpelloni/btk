#include <QtCore/BTKCore>

int main()
{
   BtkString text = "btk";
   BtkSharedPointer<int> value(new int(42));

   return text.empty() || ! value ? 1 : 0;
}
