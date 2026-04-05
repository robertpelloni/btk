# CsScript Bridge Header Frontier — 2026-04-05

## Summary
The latest Stage A `CsScript` compatibility pass changed the dominant recovery problem.

The highest-leverage engine-glue work in `qscriptengine_p.h`, `qscriptengine.cpp`, and the declarative-class conversion layer is no longer the main compile frontier.

The remaining work is now dominated by **restored bridge/header types that were built against an older JavaScriptCore object model**.

## Newly exposed frontier

### Variable-object drift
The current JavaScriptCore `JSVariableObject` contract differs from the historical Script bridge assumptions in several important ways:
- `JSVariableObjectData` is no longer available in the old form
- `JSVariableObject::d` is no longer available in the old form
- subclasses must satisfy `isDynamicScope(bool &requiresDynamicChecks) const`
- register/symbol-table ownership is modeled differently

This directly affects:
- `src/script/bridge/qscriptactivationobject_p.h/.cpp`
- `src/script/bridge/qscriptstaticscopeobject_p.h/.cpp`

### Structure creation drift
Historical Script bridge wrappers still assume an older `Structure::create(...)` shape that accepted different arguments.

This now surfaces in:
- `src/script/bridge/qscriptobject_p.h`
- `src/script/bridge/qscriptqobject_p.h`
- `src/script/bridge/qscriptstaticscopeobject_p.h`

### Visit/marking flag drift
Older wrapper/object headers still refer to flag names such as:
- `OverridesMarkChildren`

Those contracts no longer line up with the current tree and now fail in:
- `src/script/bridge/qscriptobject_p.h`
- `src/script/bridge/qscriptqobject_p.h`
- `src/script/bridge/qscriptstaticscopeobject_p.h`

## Architectural reading
This is a useful recovery milestone because it means Script restoration is now split into two clearer tracks:

### Track A — Script bridge/header modernization
Primary next work:
- update restored Script wrapper/object subclasses to current JavaScriptCore object/structure contracts
- replace historical per-class storage patterns that depended on removed `JSVariableObjectData` / `d` layout assumptions

### Track B — JavaScriptCore target/export plumbing
Separate from bridge header modernization, the direct build now also exposes:
- `JSString.cpp` `dllimport` / `s_info` linkage mismatches

That looks more like a target/export visibility issue than a Script bridge API issue.

## Recommended patch order
1. `qscriptobject_p.h`
2. `qscriptstaticscopeobject_p.h/.cpp`
3. `qscriptactivationobject_p.h/.cpp`
4. `qscriptqobject_p.h`
5. only then revisit any remaining declarative bridge fallout
6. investigate `JSString.cpp` linkage mismatch as a separate track in parallel or immediately after the bridge headers stop dominating the build

## Why this matters for BML
This frontier is still upstream of BML runtime revival.

The BML/legacy declarative bridge can only become meaningfully buildable once these Script bridge/object contracts stop failing at the JavaScriptCore substrate layer.
