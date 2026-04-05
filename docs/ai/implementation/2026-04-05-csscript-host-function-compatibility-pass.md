# CsScript Host-Function Compatibility Pass - Implementation

## Summary
This pass focused on removing the recovered Script layer's dependence on historical JavaScriptCore helper classes that no longer exist in BTK's embedded WebKit snapshot.

## Files changed
### Embedded JavaScriptCore compatibility adapters
- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/NativeFunctionWrapper.h`
- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/PrototypeFunction.h`
- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSGlobalObject.h`

### Recovered Script updates
- `src/script/api/qscriptengine.cpp`
- `src/script/api/qscriptvalueiterator.cpp`
- `src/script/bridge/qscriptactivationobject.cpp`
- `src/script/bridge/qscriptfunction.cpp`
- `src/script/bridge/qscriptfunction_p.h`
- `src/script/bridge/qscriptglobalobject.cpp`
- `src/script/bridge/qscriptglobalobject_p.h`
- `src/script/bridge/qscriptobject.cpp`

## Key implementation details
### 1. Reintroduced `NativeFunctionWrapper`
Added a compatibility wrapper that subclasses `JSC::JSFunction` and adapts old QtScript-era host functions to the current JSC host-call ABI.

Legacy signature expected by restored Script code:
- `JSValue (ExecState *, JSObject *, JSValue, const ArgList &)`

Current JSC host-call ABI expected by `CallData::native.function`:
- `EncodedJSValue (ExecState *)`

The compatibility wrapper reconstructs the missing historical call context from `ExecState`:
- `callee` -> `exec->callee()`
- `thisValue` -> `exec->hostThisValue()`
- `args` -> `ArgList args(exec)`

### 2. Reintroduced `PrototypeFunction`
Added a thin compatibility subclass over `NativeFunctionWrapper` so restored Script code can keep constructing prototype-attached host functions in the same way it historically did.

### 3. Restored `prototypeFunctionStructure()` accessor
Added a compatibility accessor on `JSC::JSGlobalObject`:
- `prototypeFunctionStructure()` now forwards to `functionStructure()`

This avoided a broad rewrite of the recovered Script layer at this stage.

### 4. Reworked `qscriptfunction.cpp`
Updated the restored wrapper bridge to match the current host-call ABI:
- proxy call/construct entry points now return `EncodedJSValue`
- wrappers reconstruct `ArgList` from `ExecState`
- returned `JSValue` is encoded explicitly via `JSValue::encode(...)`
- stale debugger-exit helper assumptions were removed from this local bridge

### 5. Tightened local JSC drift fixes discovered during the pass
Additional low-risk contractions included:
- `qscriptvalueiterator.cpp`: return `QString` through `QScript::convertToString(...)`
- `qscriptactivationobject.cpp`: mark `WriteBarrier` fields via `markStack.append(&m_delegate)`
- `qscriptobject.cpp`:
  - use `markStack.append(&d->data)` for `JSValue` storage
  - replace missing base `compareToObject(...)` fallback with local identity comparison
- `qscriptglobalobject_p.h/.cpp`:
  - modernized `GlobalObject` construction around current `JSGlobalObject(JSGlobalData&, Structure*)`
  - converted stored object references to `WriteBarrier<...>` members
  - updated visit/forwarding paths to use `.get()` and address-based barrier appends
- `qscriptengine.cpp`:
  - construct `QScript::GlobalObject` with `*globalData`

## Validation result
A direct MSVC build against:
- `build-vs2019-script-probe5/src/script/CsScript.vcxproj`

now progresses beyond the previous missing-substrate failures and reaches a newer declarative bridge frontier.

## New observed fatal frontier
The probe build now reports the next reduced hard failures primarily in:
- `src/script/bridge/qscriptdeclarativeobject.cpp`

Representative errors:
- legacy call helper assigned directly into `callData.native.function`
- legacy `JSC::TypeError` enum use with `throwError(...)`

## Interpretation
This is a good reduction:
- the missing compatibility substrate is no longer the first blocker,
- the build moved deeper,
- the next work is now concentrated in remaining declarative/class-object call semantics.
