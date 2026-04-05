#include <QtCore/QStringList>
#include <QtSql/QSqlDatabase>

int main()
{
   QSqlDatabase db;

   return db.isValid() || db.isOpen() || ! QSqlDatabase::connectionNames().isEmpty() ? 1 : 0;
}
