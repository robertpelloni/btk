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

### Narrow real integration API
Added to `QApplication`:
- `setBtkFocusTokens(...)`
- `btkFocusTokens()`
- `setBtkOwnerContext(...)`
- `btkOwnerId(...)`
- `btkSurfaceId(...)`
- `btkActivePopupOwnerId()`
- `btkActiveModalOwnerId()`
- `btkDescribeWidgetContext(...)`
- `btkDescribeFocusDecision(...)`
- `btkFocusDiagnostics()`
- `btkWouldBlockFocusChange(...)`

### Popup/modal integration progress
The BTK owner model now also affects:
- `QApplicationPrivate::tryModalHelper(...)`
- `QApplicationPrivate::isWindowBlocked(...)`
- `QApplicationPrivate::openPopup(...)`
- `QApplicationPrivate::closePopup(...)`

Current behavior:
- popups can permit same-owner interactions without being treated as globally exclusive by default
- modal widget windows do not automatically block other windows owned by the same BTK owner
- popup restoration prefers the most recent popup belonging to the same BTK owner
- opening a first popup no longer forces focus-out on an unrelated owner by default

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

## Integration progress in hotspot
This session now threads BTK policy into three real paths in that file:
- direct focus changes via `setFocusWidget(...)`
- popup/modal admission checks via `tryModalHelper(...)`
- modal blocking checks via `isWindowBlocked(...)`

## Overlay groundwork added
A first reusable diagnostics adapter now exists in QtGui:
- `src/gui/kernel/btkfocusdiagnostics.h`
- `src/gui/kernel/btkfocusdiagnostics.cpp`

This adapter packages current BTK owner/focus state into a structured snapshot suitable for future overlays and debugging tools.

## First visual overlay scaffold added
A first minimal visual overlay widget now exists:
- `src/gui/widgets/btkfocusoverlay.h`
- `src/gui/widgets/btkfocusoverlay.cpp`

This widget renders the current BTK diagnostics as an always-on-top developer-facing text surface and provides a concrete starting point for richer overlays.

## Recommended next step
Refine the overlay from a text scaffold into a richer developer HUD while continuing to improve mixed-owner concurrent popup behavior.
