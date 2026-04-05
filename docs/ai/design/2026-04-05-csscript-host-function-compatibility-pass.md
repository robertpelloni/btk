# CsScript Host-Function Compatibility Pass

## Context
The previous reduced Stage A `CsScript` frontier had reached a historically removed substrate: legacy QtScript-era code assumed the presence of JavaScriptCore helper types `PrototypeFunction` and `NativeFunctionWrapper`, plus a convenience accessor `JSGlobalObject::prototypeFunctionStructure()`.

That substrate no longer exists in the embedded WebKit/JavaScriptCore snapshot used by BTK. The recovered Script layer still referenced it in multiple places:

- `src/script/bridge/qscriptfunction_p.h/.cpp`
- `src/script/api/qscriptengine.cpp`
- `src/script/bridge/qscriptvariant.cpp`
- `src/script/bridge/qscriptqobject.cpp`
- nearby delegate/object wrappers that assumed the older host-function calling convention

## Decision
Instead of mass-rewriting every remaining recovered Script site at once, BTK now adds a narrow compatibility bridge inside the embedded JavaScriptCore tree:

- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/NativeFunctionWrapper.h`
- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/PrototypeFunction.h`
- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSGlobalObject.h` gains a compatibility accessor:
  - `prototypeFunctionStructure()` -> `functionStructure()`

This keeps the restored QtScript/CsScript code closer to its historical shape while still targeting the current JSC host-call ABI.

## Why this path
### 1. Lower blast radius
The recovered Script layer still spans many files. A focused compatibility adapter is safer than rewriting every site that still expects the old QtScript-era helper classes.

### 2. Compiler-guided contraction
The current recovery strategy is explicitly compiler-driven. Adding a narrow adapter lets MSVC tell us which remaining call/construct sites still need semantic updates.

### 3. Honest incrementalism
This does **not** claim full QtScript or Qt6 parity. It is a compatibility contraction step that moves the recovered Script layer deeper into compilation and narrows the next frontier.

## What changed semantically
The new wrappers translate old-style legacy signatures:

- legacy form: `JSValue (ExecState *, JSObject *, JSValue, const ArgList &)`
- current host ABI: `EncodedJSValue (ExecState *)`

The compatibility wrapper reconstructs:

- `callee` via `exec->callee()`
- `thisObject` via `exec->hostThisValue()`
- `args` via `ArgList args(exec)`

This makes old recovered wrapper code callable again without forcing the entire Script subtree to adopt the current JSC host-function style immediately.

## Immediate outcome
The new pass eliminated the missing-header / missing-substrate blocker and allowed `build-vs2019-script-probe5` to progress past:

- missing `PrototypeFunction.h`
- missing `NativeFunctionWrapper.h`
- missing `prototypeFunctionStructure()` helper
- older `qscriptfunction.cpp` host-call signatures
- older global-object construction assumptions
- several remaining `WriteBarrier` and string conversion mismatches in the same local area

## New reduced frontier after this pass
The next fatal compile cluster observed in the probe build is now concentrated in legacy declarative bridge call plumbing, especially:

- `src/script/bridge/qscriptdeclarativeobject.cpp`

Representative remaining mismatches include:

- assigning old-style `call(...)` helpers directly into `callData.native.function`
- old `JSC::TypeError` enum-based `throwError(...)` assumptions

This is a materially smaller and more localized frontier than the previous missing-substrate state.

## Relationship to the larger product goal
The user goal of BTK achieving deep Qt6/BML/ecosystem parity remains a **long-term program**, not something that can be truthfully claimed as complete today.

This pass supports that program by:

1. keeping recovered Script bring-up moving,
2. preserving the BML bootstrap path,
3. narrowing the remaining private-API drift surface,
4. reducing the amount of legacy declarative glue that still blocks experimental Script-backed declarative revival.
