# CsScript Private API Adaptation Inventory — 2026-04-05

## Purpose
Catalog the first meaningful private-API adaptation hotspots revealed by the direct `CsScript` build so BTK can move from generic failure logs to a structured recovery program.

## Scope
This inventory is based on the first direct `CsScript` MSVC compile diagnostics after:
- Stage A source restoration
- JavaScriptCore path contraction
- Script macro/export compatibility restoration

## Hotspot ranking
### Tier 1 — Core engine/private integration
1. `src/script/api/qscriptengine_p.h`
2. `src/script/bridge/qscriptactivationobject_p.h/.cpp`
3. `src/script/bridge/qscriptstaticscopeobject_p.h/.cpp`
4. `src/script/bridge/qscriptobject_p.h`
5. `src/script/bridge/qscriptqobject_p.h`
6. `src/script/bridge/qscriptdeclarativeclass.cpp`
7. `src/script/bridge/qscriptdeclarativeobject.cpp`

These files dominate the first compile barrier and sit directly on the boundary between recovered Script code and current embedded JavaScriptCore internals.

## Adaptation categories
### Category A — Current equivalent clearly exists
These are not “missing subsystem” problems anymore. They are remap or refactor problems.

Examples:
- historical `runtime/MarkStack*` expectations -> current `heap/MarkStack*`
- historical `runtime/StringBuilder.h` expectation -> current `wtf/text/StringBuilder.h`
- historical identifier-table access via `JSC::setCurrentIdentifierTable(...)` -> current `wtfThreadData().setCurrentIdentifierTable(...)` visible in WebKit's `API/APIShims.h` and `WTFThreadData.h`

### Category B — Current equivalent likely exists but the contract shape drifted
Examples from first compile errors:
- `JSC::UString` access patterns (`data()`, `size()`, `rep()`, `from(...)`)
- `jsNumber(...)` / `jsString(...)` call signatures
- `Structure::create(...)`
- `JSVariableObject` construction and symbol-table update helpers
- `WriteBarrier` / GC-marking contracts

These are the most important Stage A engineering tasks.

### Category C — Historical private API likely became obsolete
Examples:
- debugger callbacks like `scriptLoad` / `scriptUnload`
- older `JSVariableObjectData` layering
- `OverridesMarkChildren` flag usage in the old style

These may need removal or translation rather than one-to-one replacement.

### Category D — Needs targeted source study
Examples:
- recovered declarative bridge logic in `qscriptdeclarativeclass.cpp`
- conversion paths depending on older `Identifier` and `UString` representation details
- bridge code that mixes recovered Script wrappers with current JavaScriptCore object/value construction semantics

## Most informative findings from this pass
### 1. The build is no longer blocked by missing files
That problem has been solved at Stage A path-contract level.

### 2. The remaining failures are concentrated in private bridge code
This means BML revival is back to where we expected it would eventually land: the difficult part is not naming or file presence, but adapting the old private runtime bridge to a newer embedded engine.

### 3. `qscriptengine_p.h` is the highest-leverage adaptation file
It touches:
- identifier table setup
- debugger feedback
- `UString` conversions
- JS primitive creation helpers
- exception handling

Fixing or partially modernizing this file will likely unblock a large amount of downstream compile noise.

## Recommendation
Treat the next pass as a **symbol-contract adaptation study**, not a broad build attempt.
For each Tier 1 hotspot, record:
- old symbol/contract used
- whether a current equivalent exists
- whether the difference is rename, signature drift, visibility drift, or semantic obsolescence
- whether the next action is patch, prune, or defer
