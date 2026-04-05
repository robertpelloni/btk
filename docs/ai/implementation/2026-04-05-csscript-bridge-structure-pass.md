# CsScript Bridge Structure Pass — 2026-04-05

## Goal
Continue Stage A `CsScript` recovery by modernizing the restored bridge/object wrappers around the current JavaScriptCore object model, and remove the newly exposed `JSString.cpp` linkage/export blocker so the direct MSVC build can progress deeper into JavaScriptCore compilation.

## Files updated
- `src/script/CMakeLists.txt`
- `src/script/api/qscriptengine.cpp`
- `src/script/api/qscriptengine_p.h`
- `src/script/bridge/qscriptactivationobject.cpp`
- `src/script/bridge/qscriptactivationobject_p.h`
- `src/script/bridge/qscriptdeclarativeclass.cpp`
- `src/script/bridge/qscriptglobalobject.cpp`
- `src/script/bridge/qscriptglobalobject_p.h`
- `src/script/bridge/qscriptobject.cpp`
- `src/script/bridge/qscriptobject_p.h`
- `src/script/bridge/qscriptqobject.cpp`
- `src/script/bridge/qscriptqobject_p.h`
- `src/script/bridge/qscriptstaticscopeobject.cpp`
- `src/script/bridge/qscriptstaticscopeobject_p.h`
- `src/script/bridge/qscriptvariant.cpp`
- `src/script/bridge/qscriptvariant_p.h`

## What changed

### 1. Enabled JavaScriptCore/WTF build-side export defines inside `CsScript`
The previous direct build had progressed far enough to expose a separate JavaScriptCore target/export issue:
- `runtime/JSString.cpp` reported `dllimport` / `s_info` linkage mismatches

That failure was distinct from Script bridge drift. It indicated the recovered `CsScript` target was compiling embedded JavaScriptCore sources without the same build-side visibility assumptions those sources expect.

Applied fix in `src/script/CMakeLists.txt`:
- added `-DBUILDING_JavaScriptCore=1`
- added `-DBUILDING_WTF=1`

This removed the previously dominant `JSString.cpp` linkage blocker from the direct build probe.

### 2. Reworked restored Script wrapper structures away from `PassRefPtr<Structure>` / `RefPtr<Structure>` assumptions
The recovered Script bridge code still treated JSC `Structure` objects as if they were being passed around by `PassRefPtr`/`RefPtr` in the same way as older historical snapshots.

That no longer matched the current tree cleanly in the restored wrapper layer.

Applied adaptation:
- migrated restored Script wrapper APIs toward raw `JSC::Structure *` where those wrappers own no separate ref-counting contract
- updated corresponding engine-side structure members from `WTF::RefPtr<JSC::Structure>` to raw `JSC::Structure *`
- updated construction call sites accordingly

This was applied across:
- `QScriptObject`
- `QScriptStaticScopeObject`
- `QObjectPrototype`
- `QMetaObjectWrapperObject`
- `QMetaObjectPrototype`
- `QVariantPrototype`
- wrapper structure members in `QScriptEnginePrivate`

### 3. Modernized Script object wrappers to current `visitChildren(...)` contracts
The restored bridge/object code still used older `markChildren(...)`-style overrides and old flag names like `OverridesMarkChildren`.

Current JSC uses:
- `visitChildren(...)`
- `OverridesVisitChildren`

Applied adaptation:
- `QScriptObject` now overrides `visitChildren(...)`
- `QMetaObjectWrapperObject` now overrides `visitChildren(...)`
- `QtFunction` now overrides `visitChildren(...)`
- Script wrapper structure flags were updated from `OverridesMarkChildren` to `OverridesVisitChildren`
- delegate fallback paths now call `JSObject::visitChildren(...)`

### 4. Updated Script wrapper base classes/constructors to current object-layout expectations
The restored wrapper/object code still used older construction patterns that no longer matched the current JSC object types.

Applied adaptation:
- `QScriptObject` now derives from `JSC::JSNonFinalObject`
- `QMetaObjectWrapperObject` now derives from `JSC::JSNonFinalObject`
- constructors were updated to use current `JSGlobalData` + `Structure *` entry points instead of stale wrapper assumptions
- `QScriptObject` construction call sites now pass the active `JSGlobalData`

This contracts both the object-layout drift and the wrapper-structure ownership mismatch at the same time.

### 5. Rebuilt restored `JSVariableObject` subclasses around the current base contract
The previous pass exposed that restored Script variable-scope wrappers were still built around removed contracts such as:
- `JSVariableObjectData`
- `JSVariableObject::d`
- old `isDynamicScope()` signature

Applied adaptation:

#### `QScriptStaticScopeObject`
- replaced the old `Data : JSVariableObjectData` layering with direct members:
  - `JSC::SymbolTable m_symbolTable`
  - `bool m_canGrow`
  - `int m_registerArraySize`
- updated constructors to call the current `JSVariableObject(globalData, structure, symbolTable, registers)` base constructor
- changed `markChildren(...)` to `visitChildren(...)`
- changed `isDynamicScope()` to `isDynamicScope(bool &requiresDynamicChecks) const`
- rewired register-array growth to the current inherited `m_registerArray` / `setRegisters(...)` model

#### `QScriptActivationObject`
- removed the old `QScriptActivationObjectData : JSVariableObjectData` dependency
- replaced it with direct members:
  - `JSC::SymbolTable m_symbolTable`
  - `JSC::WriteBarrier<JSC::JSObject> m_delegate`
- updated the base constructor call to current `JSVariableObject(...)`
- added `visitChildren(...)` to mark the delegate
- updated `isDynamicScope(bool &requiresDynamicChecks) const`
- updated getter/setter override signatures to match current `JSObject` contracts with explicit attributes

### 6. Updated Script global/wrapper helpers that depended on the same stale object contracts
To keep the wrapper layer coherent, additional nearby fixes were applied in:
- `qscriptglobalobject_p.h/.cpp`
- `qscriptdeclarativeclass.cpp`
- `qscriptvariant.cpp/.h`

These changes mostly aligned constructor signatures and wrapper instantiation with the raw `Structure *` + current object model transition above.

## Net effect
This pass produced two concrete improvements:

### Improvement A — the `JSString.cpp` linkage blocker is gone
The prior fatal export/linkage mismatch around `JSString.cpp` no longer appeared in the direct MSVC build after the `BUILDING_JavaScriptCore` / `BUILDING_WTF` target-definition fix.

### Improvement B — the restored Script build now progresses substantially deeper
After the bridge/object modernization plus target-definition fix, the direct `CsScript` project build progressed well beyond the previous frontier and continued compiling many deeper JavaScriptCore runtime/JIT sources.

Observed later-stage translation units included, among others:
- `CallFrame.cpp`
- `ExecutableAllocatorFixedVMPool.cpp`
- `ExecutableAllocator.cpp`
- `JITArithmetic.cpp`
- `JITCall.cpp`
- `JIT.cpp`
- `JITOpcodes.cpp`
- `JITPropertyAccess.cpp`
- `JITStubs.cpp`
- `MathObject.cpp`
- `NativeErrorConstructor.cpp`
- `NativeErrorPrototype.cpp`
- `NumberConstructor.cpp`
- `NumberObject.cpp`

That is a strong signal that the recovered `CsScript` substrate is now materially healthier than it was at the start of the pass.

## Current reading
At this point, the restored Script recovery track has crossed another important threshold:
- the first engine-glue drift cluster is contracted
- the first bridge-header/object-model drift cluster is contracted
- the first target/export mismatch cluster is contracted
- the direct build now spends meaningful time in deeper embedded JavaScriptCore compilation rather than failing immediately in the restored Script wrapper layer

## Remaining uncertainty
This pass did **not** yet prove a full successful `CsScript` build.
The direct project build continued compiling until the validation timeout was reached, so the next unreduced fatal blocker beyond the newly fixed layers has not yet been fully captured in this pass.

## Recommended next step
1. Re-run the same direct `CsScript` MSVC build probe with a longer capture window or persisted log.
2. Record the first post-`JSString` fatal diagnostic, if any.
3. Only after that, decide whether the next recovery target is:
   - another restored Script bridge wrapper, or
   - a deeper embedded JavaScriptCore target/configuration issue.
