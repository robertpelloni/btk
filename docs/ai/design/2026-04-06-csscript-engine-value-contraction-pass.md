# CsScript Engine/Value Contraction Pass

## Context
The previous reduced Stage A frontier was concentrated in `src/script/api/qscriptengine.cpp`.
After the detached `probe5` diagnostic capture, the highest-leverage next move was to contract the engine/runtime and public value-layer drift that still prevented the compiler from reaching the next bridge hotspot.

## What this pass targeted
This pass deliberately focused on the parts of restored Script that were both:
- still close to the engine-layer frontier,
- and relatively mechanical to modernize against current embedded JavaScriptCore.

The targeted surfaces were:
- `src/script/api/qscriptengine.cpp`
- `src/script/api/qscriptprogram.cpp`
- `src/script/api/qscriptprogram_p.h`
- `src/script/api/qscriptvalue.cpp`
- nearby compatibility helpers in:
  - `src/3rdparty/webkit/Source/JavaScriptCore/runtime/NativeFunctionWrapper.h`
  - `src/3rdparty/webkit/Source/JavaScriptCore/runtime/PrototypeFunction.h`
  - `src/script/bridge/qscriptvariant.cpp`
  - `src/script/bridge/qscriptobject.cpp`

## Main design decisions

### 1. Prefer current JSC free/helper functions over historical instance-style calls
Recovered code still used older forms such as:
- `callee.getCallData(...)`
- `callee.getConstructData(...)`
- `jsNumber(exec, value)`

Current JSC in this tree expects:
- `JSC::getCallData(...)`
- `JSC::getConstructData(...)`
- `JSC::jsNumber(value)`

This pass normalizes to the current contracts rather than re-introducing more compatibility shims than necessary.

### 2. Treat `EvalExecutable` as GC-owned, not ref-counted
The restored Script layer still carried `RefPtr<EvalExecutable>` assumptions.
Current `EvalExecutable::create(...)` in this tree returns a GC-allocated object rather than a ref-counted one.

Accordingly, this pass moves the recovered `QScriptProgramPrivate` executable cache back to a raw `JSC::EvalExecutable *`.

### 3. Use Script-side `QString`/`UString` bridges consistently
Recovered code still mixed direct `QString` construction from `UString` and direct `UString` construction from `QString` in places where the current BTK/CopperSpice string layer requires the explicit bridges already established earlier:
- `QScript::toUString(...)`
- `QScript::convertToString(...)`
- `QScript::toIdentifier(...)`

This pass continues the policy that all newly touched conversions go through these helpers.

### 4. Soften missing legacy debugger/eval hooks instead of emulating them blindly
The old QtScript-era engine path expected evaluation hooks like:
- `Debugger::exceptionThrow(...)`
- `Debugger::evaluateStop(...)`
- `ExecState::setException(...)`
- `EvalExecutable::isCompiled()`
- older interpreter execute overloads with explicit exception out-params

This pass replaces those with the narrower current equivalents that actually exist in-tree:
- direct `globalData().exception` assignment
- `Debugger::exception(...)`
- direct `Interpreter::execute(...)`
- compile-on-demand without the removed `isCompiled()` probe

That keeps the recovery honest and compiler-driven without inventing fake runtime semantics.

## Architectural outcome
This pass materially reduces the engine/value-layer mismatch surface and changes the reduction shape of the Stage A compiler frontier:
- the build no longer stops first in the earlier `qscriptengine.cpp` / `qscriptvalue.cpp` clusters,
- the next concentrated hotspot is now primarily `src/script/bridge/qscriptqobject.cpp`,
- the remaining engine/value issues are now smaller trailing conversion/template mismatches rather than the dominant first-failure wall.

## Remaining frontier after this pass
The current first-failure cluster is now dominated by `qscriptqobject.cpp` with recurring themes:
- removed enum-style error dispatch (`TypeError`, `GeneralError`, `SyntaxError`)
- old host-call function pointer contracts
- `MarkStack::append(...)` barrier/root handling drift
- removed `Heap::isCellMarked(...)`
- old `JSValue::getCallData(...)` usage
- `info` → `s_info` metadata drift
- `QString` / `UString` conversion cleanup still needed across bridge code

## Why this matters for BML revival
BML bring-up is still blocked on broader Script recovery, but this pass is meaningful because it removes a large chunk of the public engine/value substrate mismatch before returning to the larger QObject bridge layer that declarative/BML ultimately depends on.