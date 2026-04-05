# BML Bootstrap Implementation — 2026-04-05

## Implemented
### Top-level build gate
- Added `Declarative` to the optional BTK component list.
- Kept `WITH_DECLARATIVE` defaulted to `OFF` so current default build behavior is preserved.

### Target alias bootstrap
- Added `BTK::Bml` and `BtkBml` aliases when `CsDeclarative` is present.
- Added downstream package-side `BTK::Bml` alias creation in `cmake/BTKConfig.cmake`.

### Public BML alias headers
Added branding-first alias headers in `src/declarative/`:
- `src/declarative/qml/bml.h`
- `src/declarative/qml/bmlengine.h`
- `src/declarative/qml/bmlcomponent.h`
- `src/declarative/qml/bmlcontext.h`
- `src/declarative/qml/bmlerror.h`
- `src/declarative/util/bmlview.h`

### Header exposure wiring
Updated declarative CMake include lists so these BML headers become part of the declarative public surface whenever the declarative component is built.

## Important finding
`src/declarative/` is not a drop-in modern Qt Quick 2 stack.
It is a legacy `QtDeclarative` / QML1-style subsystem and it still depends heavily on `QtScript` / `QScript*` APIs.

That means this bootstrap is intentionally:
- a naming and packaging step
- not a complete modern declarative runtime delivery

## Result
BTK now has a concrete first-pass answer to "call QML in BTK BML":
- the declarative naming surface can be presented as **BML**
- package aliases are reserved for `BTK::Bml`
- the build remains safe because the legacy declarative component is not forced on by default
