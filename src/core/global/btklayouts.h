/***********************************************************************
*
* BTK Layout type aliases.
* Clean BTK-branded API surface over Qt layout types.
*
***********************************************************************/

#ifndef BTK_LAYOUT_ALIASES_H
#define BTK_LAYOUT_ALIASES_H

#include <qlayout.h>
#include <qboxlayout.h>
#include <qgridlayout.h>
#include <qformlayout.h>
#include <qstackedlayout.h>
#include <qsizepolicy.h>

using BTKLayout       = QLayout;
using BTKHBoxLayout   = QHBoxLayout;
using BTKVBoxLayout   = QVBoxLayout;
using BTKGridLayout   = QGridLayout;
using BTKFormLayout   = QFormLayout;
using BTKStackedLayout = QStackedLayout;
using BTKSizePolicy   = QSizePolicy;

#endif
