# CsScript First-Compile Gap Analysis — 2026-04-05

## Purpose
Interpret the first direct `CsScript` compiler diagnostics after the Stage A recovery/bootstrap and drift-contraction passes.

## Important milestone
BTK is now past the path-recovery phase.
The first direct `CsScript` compile reached real compiler diagnostics.

This is significant because the dominant uncertainty is no longer:
- missing source provenance
- missing build wiring
- missing JavaScriptCore file paths

It is now:
- **API and ABI drift between historical `CsScript` code and the current embedded JavaScriptCore snapshot**

## What happened in this pass
A direct build of:
- `build-vs2019-script-probe3/src/script/CsScript.vcxproj`

with project references disabled reached Script-specific compilation and surfaced concrete failures.

## First-layer compatibility fixes already applied
Before the deeper diagnostics were visible, BTK needed two compatibility shims restored:

### 1. Export macro compatibility
Current BTK no longer defined historical Script export macros.
This pass restored:
- `Q_SCRIPT_EXPORT`

inside:
- `src/core/global/qexport.h`

### 2. Meta-object macro compatibility
Current BTK no longer defined historical Script object/signal/slot helper aliases.
This pass restored:
- `SCRIPT_CS_OBJECT`
- `SCRIPT_CS_OBJECT_MULTIPLE`
- `SCRIPT_CS_GADGET`
- `SCRIPT_CS_SLOT_*`
- `SCRIPT_CS_SIGNAL_*`

inside:
- `src/core/kernel/csobject_macro.h`

### 3. Public header visibility compatibility
Several restored Script headers were updated to include:
- `qglobal.h`

so that export and supporting core macros are visible in current BTK.

## Result after first-layer fixes
The compiler progressed beyond the original parse collapse and began exposing deeper incompatibilities.
That is progress.

## Main incompatibility clusters revealed
### Cluster A — JavaScriptCore private API drift
A large set of errors in:
- `src/script/api/qscriptengine_p.h`
- `src/script/bridge/qscriptactivationobject_p.h/.cpp`
- `src/script/bridge/qscriptstaticscopeobject_p.h/.cpp`
- `src/script/bridge/qscriptobject_p.h`
- `src/script/bridge/qscriptqobject_p.h`
- `src/script/bridge/qscriptdeclarativeclass.cpp`
- `src/script/bridge/qscriptdeclarativeobject.cpp`

show that the restored Script code expects older JavaScriptCore internals that no longer match the current embedded tree.

Representative symptoms include missing or changed members/functions such as:
- `setCurrentIdentifierTable`
- `scriptLoad` / `scriptUnload`
- `UString::data()` / `UString::size()` / `UString::rep()` / `UString::from(...)`
- `JSVariableObjectData`
- `OverridesMarkChildren`
- older `Structure::create(...)` and `symbolTablePut...(...)` contracts
- older `jsNumber(...)` / `jsString(...)` calling conventions

This strongly suggests a historical Script layer built against an older JavaScriptCore private API shape than the current WebKit-embedded snapshot provides.

### Cluster B — Namespace/export linkage drift inside current JavaScriptCore build mode
The build also surfaced linkage/export-style issues such as:
- `JSString::s_info`: definition of dllimport static data member not allowed

This indicates some current Script build definitions or symbol-visibility expectations are still mismatched relative to how the current JavaScriptCore sources expect to be built inside BTK.

### Cluster C — Restored Script bridge code assumes older object-memory/layout contracts
Errors around:
- `JSVariableObject`
- `MarkStack`
- `WriteBarrier`
- `Identifier`
- `NativeFunction`

suggest that several bridge classes are tightly coupled to the old JavaScriptCore object model and GC/barrier API.
This is likely where the most careful adaptation work will be required.

## Strategic interpretation
The first compiler pass validates a critical architectural point:

> Stage A path recovery succeeded.
> The next challenge is not file recovery but **private engine adaptation**.

This means the project should now move from:
- source-list contraction

to:
- targeted compatibility adaptation between recovered Script bridge code and the current embedded JavaScriptCore private API

## Recommended next move
Prioritize a **private API adaptation inventory** for these hotspots:
1. `src/script/api/qscriptengine_p.h`
2. `src/script/bridge/qscriptactivationobject*`
3. `src/script/bridge/qscriptstaticscopeobject*`
4. `src/script/bridge/qscriptobject*`
5. `src/script/bridge/qscriptqobject*`
6. `src/script/bridge/qscriptdeclarativeclass*`

That inventory should map each failing historical JavaScriptCore symbol or contract to one of:
- current equivalent exists
- current equivalent exists with changed signature/layout
- obsolete and removable
- unclear / requires deeper WebKit-source study
