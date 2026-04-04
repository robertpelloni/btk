# BTK Multi-User Scaffolding Session Notes

## Implemented
### Core public types
- `src/core/kernel/btkinputowner.h`
- `src/core/kernel/btkinputowner.cpp`
- `src/core/kernel/btkfocustoken.h`
- `src/core/kernel/btkfocustoken.cpp`

### GUI public types
- `src/gui/kernel/btkinputarbitrator.h`
- `src/gui/kernel/btkinputarbitrator.cpp`

### Build integration
- updated `src/core/kernel/kernel.cmake`
- updated `src/gui/kernel/kernel.cmake`

## Public API Added
### QtCore include surface
- `<QtCore/BTKInputOwner>`
- `<QtCore/BTKFocusToken>`

### QtGui include surface
- `<QtGui/BTKInputArbitrator>`

## Intent
This work does not yet alter the existing focus engine. It establishes BTK-native concepts for:
- user/session ownership
- device grouping
- owner-scoped focus
- modality policy
- arbitration decisions

## Why this approach is safe
The current BTK/CopperSpice focus logic is tightly interwoven with widget, window, popup, and modality internals. Replacing it directly would be risky. Introducing explicit BTK types first allows future integration in measured steps.

## Observed hotspot
`src/gui/kernel/qapplication_cs.cpp` contains the deepest concentration of single-focus assumptions, including:
- `QApplicationPrivate::focus_widget`
- `QApplicationPrivate::active_window`
- popup focus restoration
- focus chain traversal
- active window focus transfer

That file is the main future integration hotspot for owner-scoped multi-focus behavior.

## Recommended next step
Thread `BtkFocusToken` and `BtkInputArbitrator` into a narrow, low-risk slice of GUI focus logic first—likely popup/modal arbitration or a non-invasive helper path around `QApplicationPrivate::setFocusWidget(...)`.
