# BML Bootstrap Design — 2026-04-05

## Design choice
Adopt a **branding-first, runtime-later** strategy.

Instead of attempting to land a complete modern declarative stack in one pass, BTK will:
1. expose the existing declarative surface under **BML** names
2. add package/target aliases so downstream users can think in terms of BTK/BML
3. keep the legacy declarative component disabled by default until its dependency story is repaired

## Why this design is correct
This follows the same compatibility-first pattern used elsewhere in BTK:
- `BTK::Core` aliases over `CsCore`
- BTK public alias headers over existing Cs/Qt-shaped types
- deploy wrappers instead of immediate full subsystem rewrites

For declarative/UI runtime work, the equivalent safe move is:
- **BML names first**
- **runtime modernization second**

## Bootstrap deliverables
### CMake layer
- add `Declarative` as a known optional component
- keep `WITH_DECLARATIVE` defaulted to `OFF`
- expose `BTK::Bml` as an alias to `CsDeclarative` when/if the declarative module is built

### Public header layer
Introduce BML alias headers for the central entry points:
- `BML`
- `BMLEngine`
- `BMLComponent`
- `BMLContext`
- `BMLError`
- `BMLView`

### Registration helpers
Add `bmlRegisterType(...)` helpers as thin wrappers over the current `qmlRegisterType(...)` family.

## Deferred work
The following is explicitly deferred beyond this bootstrap:
- a modern BML scene graph / Quick 2 equivalent
- BML Controls 2 equivalent
- JavaScript engine modernization away from legacy `QScript*`
- a real BML package/runtime validation matrix

## Architectural implication
This pass establishes the **name and package contract** for BML without pretending the underlying runtime is already modern.
That is a necessary intermediate step if BTK is going to evolve from legacy QtDeclarative lineage toward a future declarative runtime with its own identity.
