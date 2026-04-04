# BTK Multi-User Scaffolding Validation

## Scope
This validation covers the newly introduced BTK ownership/focus/arbitration scaffolding.

## Added surfaces to validate later in a compiler-ready environment
### QtCore
- `<QtCore/BTKInputOwner>`
- `<QtCore/BTKFocusToken>`

### QtGui
- `<QtGui/BTKInputArbitrator>`

## Recommended validation cases
1. Create two `BtkInputOwner` instances representing two local users.
2. Create multiple `BtkFocusToken` instances with different scopes and modality policies.
3. Verify `BtkInputArbitrator::route(...)` behavior for:
   - shared modality
   - owner-exclusive modality same owner
   - owner-exclusive modality different owner
   - transfer within same owner to a different surface
4. Verify public header generation exposes the new BTK include wrappers correctly.

## Known limitation
No full compiler validation was possible in this environment because CMake could not complete configuration without a usable C/C++ compiler toolchain.
