/***********************************************************************
*
* BTK SQL type aliases.
* Clean BTK-branded API surface over Qt SQL types.
*
***********************************************************************/

#ifndef BTK_SQL_ALIASES_H
#define BTK_SQL_ALIASES_H

#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qsqlrecord.h>
#include <qsqlfield.h>
#include <qsqltablemodel.h>
#include <qsqlquerymodel.h>
#include <qsqlrelationaldelegate.h>

using BTKDatabase      = QSqlDatabase;
using BTKQuery         = QSqlQuery;
using BTKSqlError      = QSqlError;
using BTKSqlRecord     = QSqlRecord;
using BTKSqlField      = QSqlField;
using BTKSqlTableModel = QSqlTableModel;
using BTKSqlQueryModel = QSqlQueryModel;

#endif
