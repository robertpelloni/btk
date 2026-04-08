/***********************************************************************
*
* BTK GUI compatibility header.
* This header layers BTK-branded aliases over the current CsGui-based
* implementation surface for safe incremental migration.
*
* Includes widget, painting, layout, input, and windowing aliases.
*
***********************************************************************/

#ifndef BTK_GUI_COMPAT_H
#define BTK_GUI_COMPAT_H

#include <qtgui.h>

#include <btkinputowner.h>
#include <btkfocustoken.h>
#include <btkwidgets.h>
#include <btklayouts.h>
#include <btkpainting.h>

// BTK re-exports the full GUI type surface under BTK naming
// Individual BTK widget/painting/layout types are available through
// the included alias headers

#endif
