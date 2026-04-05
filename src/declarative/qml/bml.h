/***********************************************************************
*
* BML compatibility umbrella for BTK's declarative runtime bootstrap.
* This is a branding-first alias layer over the existing QtDeclarative /
* QML-style API surface while the deeper runtime modernization plan is
* developed.
*
***********************************************************************/

#ifndef BML_COMPAT_H
#define BML_COMPAT_H

#include <QtDeclarative/qdeclarative.h>

#define BML_DECLARE_TYPE(TYPE) QML_DECLARE_TYPE(TYPE)
#define BML_DECLARE_TYPE_HASMETATYPE(TYPE) QML_DECLARE_TYPE_HASMETATYPE(TYPE)
#define BML_DECLARE_INTERFACE(INTERFACE) QML_DECLARE_INTERFACE(INTERFACE)
#define BML_DECLARE_INTERFACE_HASMETATYPE(INTERFACE) QML_DECLARE_INTERFACE_HASMETATYPE(INTERFACE)
#define BML_DECLARE_TYPEINFO(TYPE, FLAGS) QML_DECLARE_TYPEINFO(TYPE, FLAGS)

enum {
   BML_HAS_ATTACHED_PROPERTIES = QML_HAS_ATTACHED_PROPERTIES
};

template <typename T>
inline int bmlRegisterType()
{
   return qmlRegisterType<T>();
}

template <typename T>
inline int bmlRegisterUncreatableType(const char *uri, int versionMajor, int versionMinor,
   const char *bmlName, const QString &reason)
{
   return qmlRegisterUncreatableType<T>(uri, versionMajor, versionMinor, bmlName, reason);
}

inline int bmlRegisterTypeNotAvailable(const char *uri, int versionMajor, int versionMinor,
   const char *bmlName, const QString &message)
{
   return qmlRegisterTypeNotAvailable(uri, versionMajor, versionMinor, bmlName, message);
}

#endif
