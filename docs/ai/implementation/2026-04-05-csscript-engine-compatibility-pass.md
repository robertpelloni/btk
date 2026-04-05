# CsScript Engine Compatibility Pass — 2026-04-05

## Goal
Push the restored `CsScript` Stage A baseline past the first concentrated JavaScriptCore private-API drift cluster so the remaining failures become narrower bridge/header incompatibilities rather than broad engine glue breakage.

## Files updated
- `src/script/api/qscriptengine_p.h`
- `src/script/api/qscriptengine.cpp`
- `src/script/api/qscriptengineagent_p.h`
- `src/script/api/qscriptstring.cpp`
- `src/script/api/qscriptvalue.cpp`
- `src/script/bridge/qscriptclassobject.cpp`
- `src/script/bridge/qscriptdeclarativeclass.cpp`
- `src/script/bridge/qscriptdeclarativeobject.cpp`
- `src/script/bridge/qscriptqobject.cpp`
- `src/script/bridge/qscriptstaticscopeobject.cpp`
- `src/script/bridge/qscriptvariant.cpp`

## What changed

### 1. Identifier-table adaptation moved to current-thread API
The old Script glue expected legacy helpers such as:
- `JSC::currentIdentifierTable()`
- `JSC::setCurrentIdentifierTable(...)`

The current embedded JavaScriptCore tree stores that state in `wtfThreadData()`.

Applied fixes:
- `QScript::APIShim` now saves/restores identifier tables with `wtfThreadData().setCurrentIdentifierTable(...)`
- `QScriptEnginePrivate` bootstrap now snapshots and restores the identifier table through `wtfThreadData().currentIdentifierTable()` / `wtfThreadData().setCurrentIdentifierTable(...)`

This resolves one of the highest-confidence current-equivalent private API drifts previously identified in the adaptation matrix.

### 2. Replaced obsolete debugger-hook assumptions with Script-agent routing
The historical Script glue assumed the current JavaScriptCore `Debugger` still exposed:
- `scriptLoad(...)`
- `scriptUnload(...)`

That contract no longer exists on the JSC debugger base class.

Applied fixes:
- `UStringSourceProviderWithFeedback` no longer depends on missing `JSC::Debugger` load/unload callbacks
- feedback now routes through `QScriptEngineAgentPrivate` when an active Script agent exists
- `qscriptengineagent_p.h` was updated to match the current `Debugger::sourceParsed(...)` override shape
- agent-side `scriptLoad(...)` now explicitly converts JSC `UString` inputs into BTK/CopperSpice `QString`

### 3. Reworked Script source-provider feedback wrapper away from private constructor drift
Current JavaScriptCore makes `UStringSourceProvider` construction inaccessible to the old subclassing pattern.

Applied fix:
- `QScript::UStringSourceProviderWithFeedback` now derives from `JSC::SourceProvider` directly
- it stores its own `m_source`
- it implements `getRange(...)`, `data()`, and `length()` directly

This preserves Script-side bookkeeping while removing the old dependency on an inaccessible `UStringSourceProvider` constructor and on the old internal `m_source` layout.

### 4. Modernized core string/value helper paths in `qscriptengine_p.h`
The restored Script engine still expected older `UString` helpers such as:
- `UString::from(...)`
- `UString::data()` / `size()`
- `UString::toDouble()`
- `ExecState::setException(...)`

Applied fixes:
- introduced `QScript::toUString(const QString &)` and `QScript::toIdentifier(ExecState *, const QString &)` compatibility helpers
- `ToString(qsreal)` now uses `JSC::UString::number(...)`
- `ToNumber(const QString &)` now uses `WTF::String(value).toDouble()`
- `convertToString(...)` now converts through `QString::fromUtf16(...)` using `characters()` / `length()`
- Script value boxing now uses current `jsNumber(...)` signatures and explicit `QString` → `UString` conversion
- exception restoration now writes back through `exec->globalData().exception`
- `constructDate(...)` call sites were updated to the current `(exec, lexicalGlobalObject, args)` shape
- runtime type checks now use current `s_info` symbols where applicable (`JSArray`, `DateInstance`, `ErrorInstance`, `RegExpObject`)

### 5. Contracted declarative bridge drift enough to expose deeper bridge headers
`qscriptdeclarativeclass.cpp` previously dominated the error stream with obsolete assumptions around:
- `JSValue(exec, number)` construction
- `jsString(exec, QString)`
- `UString::Rep`
- `ustring().rep()`
- scope-chain iteration returning raw pointers
- legacy identifier-handle reconstruction

Applied fixes:
- switched primitive value construction to `jsNumber(...)`
- switched string boxing to `jsString(..., QScript::toUString(...))`
- replaced old `UString::Rep`/`.rep()` handle paths with `StringImpl *` / `Identifier::impl()`
- replaced old string reconstruction with `QScript::convertToString(...)`
- replaced old array-index extraction with `JSC::Identifier::toUInt32(...)`
- adapted scope-chain iteration to use `WriteBarrier<JSObject>::get()`
- adapted static-scope-object property identifier creation to use `QScript::toIdentifier(...)`

### 6. Propagated low-risk QString/JSC helper fixes into nearby bridge/API files
Additional low-risk compatibility updates were applied in:
- `qscriptdeclarativeobject.cpp`
- `qscriptstaticscopeobject.cpp`
- `qscriptclassobject.cpp`
- `qscriptqobject.cpp`
- `qscriptvariant.cpp`
- `qscriptstring.cpp`
- `qscriptvalue.cpp`
- `qscriptengine.cpp`

These were mostly:
- `QString` → `JSC::Identifier` adaptation
- `QString` → `JSC::UString` adaptation for `jsString(...)`
- `.rep()` → `.impl()` migration
- `UString` → `QString` conversion cleanup

## Resulting impact
This pass did not make `CsScript` build cleanly yet, but it materially changed the failure frontier.

Before this pass, the compile stream was still heavily dominated by:
- identifier-table glue drift
- debugger load/unload drift
- source-provider access drift
- broad declarative class `UString::Rep` drift
- old `jsNumber(...)` / `jsString(...)` call signatures

After this pass, the dominant remaining compile blockers are now concentrated in deeper bridge/header contracts such as:
- `src/script/bridge/qscriptobject_p.h`
- `src/script/bridge/qscriptstaticscopeobject_p.h`
- `src/script/bridge/qscriptqobject_p.h`
- `src/script/bridge/qscriptactivationobject_p.h`

with characteristic current-tree mismatches around:
- removed `JSVariableObjectData`
- removed `JSVariableObject::d`
- changed `isDynamicScope(bool &)` signature
- changed `Structure::create(...)` contract
- old `OverridesMarkChildren` flag usage

This is useful progress because the build is now telling a cleaner truth: the next work is no longer generic Script-engine glue archaeology, but a specific variable-object / object-wrapper bridge modernization task.

## New observations
A separate JavaScriptCore build/export issue is now visible in the same direct project build:
- `runtime/JSString.cpp` reports `dllimport` / `s_info` linkage mismatches when compiled under the restored `CsScript` target arrangement

That appears to be a second track, distinct from the Script bridge-header drift, and should be documented as a parallel blocker rather than conflated with the Script wrapper-layer fixes.

## Recommended next action
1. Patch the bridge headers and wrappers in this order:
   - `qscriptobject_p.h`
   - `qscriptstaticscopeobject_p.h/.cpp`
   - `qscriptactivationobject_p.h/.cpp`
   - `qscriptqobject_p.h`
2. Update each restored `JSVariableObject` subclass to the current:
   - `isDynamicScope(bool &requiresDynamicChecks) const`
   - symbol-table/register ownership model
   - `Structure::create(...)` contract
   - current visit/marking flags
3. Investigate the `JSString.cpp` `dllimport static data member` mismatch separately as a target/build-visibility issue in the recovered Script target.
