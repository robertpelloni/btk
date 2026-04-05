# CsScript Declarative/Class Call-ABI Pass - Implementation

## Summary
This pass adapted restored declarative/class delegate call plumbing to the current JavaScriptCore host-call ABI.

## Files changed
- `src/script/bridge/qscriptdeclarativeobject_p.h`
- `src/script/bridge/qscriptdeclarativeobject.cpp`
- `src/script/bridge/qscriptclassobject_p.h`
- `src/script/bridge/qscriptclassobject.cpp`
- `build-vs2019-script-probe5/run_csscript_background_wmi.ps1`

## Key implementation details
### 1. Declarative delegate call path
`DeclarativeObjectDelegate` now exposes:
- `call(ExecState *)` as the current JSC host-call entry point
- `callImpl(ExecState *, JSObject *, JSValue, const ArgList &)` as the preserved business-logic implementation

The proxy reconstructs:
- `callee` from `exec->callee()`
- `thisValue` from `exec->hostThisValue()`
- `args` from `ArgList args(exec)`

### 2. Class delegate call path
`ClassObjectDelegate` now exposes:
- `call(ExecState *)`
- `callImpl(ExecState *, JSObject *, JSValue, const ArgList &)`
- `construct(ExecState *)`
- `constructImpl(ExecState *, JSObject *, const ArgList &)`

This lets the restored class-object delegate continue using its older internal logic while satisfying the current `CallData::native.function` / `ConstructData::native.function` contracts.

### 3. Error construction modernization
Both files replaced stale enum-style patterns like:
- `throwError(exec, JSC::TypeError, "...")`

with current-style composition:
- `throwError(exec, createTypeError(exec, ...))`

using the already introduced `QScript::toUString(...)` conversion helpers.

### 4. Validation strategy
A direct MSVC probe was rerun against:
- `build-vs2019-script-probe5/src/script/CsScript.vcxproj`

The synchronous window still times out before full completion, so an additional detached helper script was written to support persisted-log background capture in the same probe directory:
- `build-vs2019-script-probe5/run_csscript_background_wmi.ps1`

## Immediate result
The build no longer reported the earlier reduced hard failures in:
- `qscriptdeclarativeobject.cpp` call registration
- `qscriptclassobject.cpp` call/construct registration
- old `JSC::TypeError` enum assumptions in that same reduced cluster

Instead, the probe advanced substantially deeper into later JavaScriptCore/API compilation work before hitting the session timeout.
