# CsScript Engine/Value Contraction Pass - Implementation

## Summary
Continued Stage A `CsScript` recovery with a focused contraction pass over the restored engine/value layer and nearby executable-host compatibility helpers.

## Safety / process preservation
- Performed another fresh process audit before the next build-validation pass.
- Did not terminate any existing background processes.
- Reused the existing probe tree `build-vs2019-script-probe5` for validation.

## Files updated
- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/NativeFunctionWrapper.h`
- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/PrototypeFunction.h`
- `src/script/api/qscriptengine.cpp`
- `src/script/api/qscriptprogram.cpp`
- `src/script/api/qscriptprogram_p.h`
- `src/script/api/qscriptvalue.cpp`
- `src/script/bridge/qscriptobject.cpp`
- `src/script/bridge/qscriptvariant.cpp`

## Implemented changes

### 1. Host-wrapper compatibility bridge widened for current native signatures
Updated the embedded compatibility wrappers so they can accept both:
- the restored legacy host-call helper signature used by older Script code,
- and the current `NativeFunction` signature used by current JSC.

Files:
- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/NativeFunctionWrapper.h`
- `src/3rdparty/webkit/Source/JavaScriptCore/runtime/PrototypeFunction.h`

This was intentionally narrow: it avoided another mass rewrite of call-site construction logic while allowing the compiler to progress deeper.

### 2. `qscriptengine.cpp` evaluation/eval-executable drift contracted
Adjusted the engine evaluation path away from removed historical APIs:
- removed `EvalExecutable::isCompiled()` assumptions
- replaced old exception writes with `exec->globalData().exception = ...`
- replaced removed debugger hooks with current `Debugger::exception(...)`
- switched interpreter eval execution to the current overload
- updated uncaught-exception line-number lookup to the current one-argument form
- updated direct `evaluate(...)` creation path to:
  - use `QScript::toUString(...)` for program/file strings
  - call `EvalExecutable::create(exec, source, false)`
  - pass a raw executable pointer instead of a ref-counted wrapper

### 3. `qscriptengine.cpp` regex/string/variant conversion cleanup
Modernized multiple remaining engine-layer conversion sites:
- `QRegularExpression` creation now uses `QScript::toUString(...)`
- `constructRegExp(...)` now uses the current `constructRegExp(exec, exec->lexicalGlobalObject(), args)` contract
- `QString` ↔ `UString` conversions now flow through `QScript::convertToString(...)` and `QScript::toUString(...)`
- `translationContextFromUrl(...)` now caches translated context via explicit conversion helpers
- `toVariant(...)` and `jscValueFromVariant(...)` no longer use the stale `jsNumber(exec, ...)` form for primitive numerics
- `toStringHandle(...)` now routes through `QScript::toIdentifier(...)`

### 4. `qscriptprogram` executable ownership corrected
Recovered `QScriptProgramPrivate` still used `RefPtr<EvalExecutable>` even though current `EvalExecutable` is GC-owned rather than ref-counted in this tree.

Adjusted:
- `src/script/api/qscriptprogram_p.h`
- `src/script/api/qscriptprogram.cpp`

to store a raw `JSC::EvalExecutable *` and stop using `.get()` / `.clear()` ref-count style operations.

### 5. `qscriptvalue.cpp` primitive/value API drift contracted
Modernized several public Script value helpers:
- replaced `jsNumber(exec, ...)` with `jsNumber(...)`
- replaced `jsString(exec, QString)` assumptions with `jsString(exec, QScript::toUString(...))`
- updated `setPrototype(...)` to the current `setPrototype(JSGlobalData&, JSValue)` form
- updated `removeDirect(...)` / `putDirect(...)` calls to pass `JSGlobalData&`
- converted `QScriptValue::toString()` through `QScript::convertToString(...)`
- routed `QString` property access through explicit `QScript::toUString(...)`
- replaced `JSValue` instance-style call/construct discovery with `JSC::getCallData(...)` / `JSC::getConstructData(...)`
- replaced stale array-argument type failures with `createTypeError(...)` + `throwError(...)`
- updated `Arguments::info` / `JSArray::info` uses to `s_info`

### 6. Local bridge cleanup in variant/object helpers
Contracted a few remaining local mismatches that were cheap and low-risk:
- removed a stale `markStack.append(&d->data)` path from `src/script/bridge/qscriptobject.cpp`
- modernized `src/script/bridge/qscriptvariant.cpp` string/error/number helper usage to current conversion and `createTypeError(...)` patterns

## Validation run
Executed direct MSVC validation against:
- `build-vs2019-script-probe5/src/script/CsScript.vcxproj`

Command form used:
- direct MSBuild invocation through Visual Studio 2019 Build Tools
- `Configuration=Release`
- `Platform=x64`
- `BuildProjectReferences=false`
- `/m:1`

## Result
This pass materially reduced the compiler frontier:
- previous observed failure volume during the same direct probe: `162` errors
- current observed failure volume after this pass: `78` errors

## New reduced first-failure shape
The dominant first-failure hotspot is no longer `qscriptengine.cpp`.
The remaining clustered frontier is now centered primarily in:
- `src/script/bridge/qscriptqobject.cpp`

Representative remaining error classes now include:
- removed `Heap::isCellMarked(...)`
- `MarkStack::append(...)` root/barrier drift
- enum-style `TypeError` / `GeneralError` / `SyntaxError` drift
- old native function pointer contracts assigned directly into current host ABI fields
- `info` → `s_info` metadata drift
- lingering `QString` / `UString` conversion mismatches in bridge code
- current `setPrototype(...)` / `getCallData(...)` contract drift in QObject bridge paths

## Interpretation
This is a real Stage A contraction pass, not just another capture pass.
It removed the previously dominant engine/value wall and exposed the next smaller bridge-focused frontier with substantially fewer total compiler errors.