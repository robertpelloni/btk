/***********************************************************************
*
* BTK umbrella compatibility header.
* This header intentionally layers BTK-branded aliases over the current
* Cs*-based implementation surface as a safe first migration step.
*
* Including this header gives access to:
* - Core framework (QCoreApplication, QString, etc.)
* - Smart pointers (BtkSharedPointer, BtkUniquePointer, etc.)
* - Signals/slots (BtkSignal::SignalBase, etc.)
* - Text/string types (BtkString, BtkChar, etc.)
* - Multi-user ownership primitives (BtkInputOwner, BtkFocusToken)
* - Core type aliases (BTKTimer, BTKThread, BTKFile, etc.)
*
***********************************************************************/

#ifndef BTK_CORE_COMPAT_H
#define BTK_CORE_COMPAT_H

#include <qglobal.h>
#include <qtcore.h>

#include <btkpointer.h>
#include <btksignal.h>
#include <btkstring.h>
#include <btkinputowner.h>
#include <btkfocustoken.h>
#include <btkcoretypes.h>

#endif
