# CsScript First Compile Diagnostics — 2026-04-05

## Goal
Capture the first direct `CsScript` build failures after Stage A recovery became path-consistent enough to configure experimentally.

## Build invocation
Used a direct MSBuild invocation against:
- `build-vs2019-script-probe3/src/script/CsScript.vcxproj`

with:
- `Configuration=Release`
- `BuildProjectReferences=false`

This ensured the build reached Script-specific compilation rather than repeatedly rebuilding the entire dependency graph.

## Preparatory compatibility fixes made in this pass
### Restored export macro compatibility
Updated:
- `src/core/global/qexport.h`

Added current compatibility definitions for:
- `Q_SCRIPT_EXPORT`

### Restored Script meta-object helper compatibility
Updated:
- `src/core/kernel/csobject_macro.h`

Added compatibility aliases for:
- `SCRIPT_CS_OBJECT`
- `SCRIPT_CS_OBJECT_MULTIPLE`
- `SCRIPT_CS_GADGET`
- `SCRIPT_CS_SLOT_*`
- `SCRIPT_CS_SIGNAL_*`

### Restored Script header macro visibility
Updated public/private Script headers to include:
- `qglobal.h`

This allowed the restored historical headers to see the export/core macro layer again in the current BTK tree.

## First meaningful compiler diagnostics
After the compatibility shim work above, the build progressed into deeper failures.

### Representative failure areas
#### `src/script/api/qscriptengine_p.h`
Errors indicate heavy JavaScriptCore private API drift, including missing or changed contracts around:
- `setCurrentIdentifierTable`
- debugger callbacks like `scriptLoad` / `scriptUnload`
- `UString` conversion/access helpers
- `jsNumber(...)`
- `jsString(...)`
- `constructDate(...)`
- exception handling paths

#### `src/script/bridge/qscriptactivationobject*`
Errors indicate old assumptions about:
- `JSVariableObjectData`
- `JSVariableObject` constructor shape
- symbol table APIs

#### `src/script/bridge/qscriptstaticscopeobject*`
Errors indicate old assumptions about:
- GC marking hooks
- structure creation
- register arrays / variable object internals

#### `src/script/bridge/qscriptobject*` and `qscriptqobject*`
Errors indicate further object-model and marking-contract drift.

#### `src/script/bridge/qscriptdeclarativeclass*`
Errors indicate older assumptions about:
- `JSValue` construction helpers
- `Identifier` construction
- `UString` representation access
- conversion between recovered Script bridge code and current JavaScriptCore types

## Current reading
The recovered `CsScript` subsystem is now at the point where:
- build wiring exists
- path drift is contracted away
- configure-time enablement works experimentally
- the remaining work is concentrated in source-level compatibility adaptation with current JavaScriptCore internals

## Immediate recommendation
Next pass should create a targeted adaptation matrix starting with:
- `qscriptengine_p.h`
- `qscriptactivationobject*`
- `qscriptstaticscopeobject*`
- `qscriptdeclarativeclass*`

These files appear to dominate the first meaningful compile barrier.
