# CHANGELOG

## 0.1.6 - 2026-04-06
- widened the embedded `NativeFunctionWrapper` / `PrototypeFunction` compatibility bridge so restored Script call sites can target both legacy wrapper signatures and current JSC native-function signatures
- contracted the restored `CsScript` engine/value frontier across `qscriptengine.cpp`, `qscriptprogram.cpp/.p.h`, and `qscriptvalue.cpp`
- replaced removed evaluation/debugger/value helpers with current JSC equivalents, including raw `EvalExecutable *` caching, current `getCallData(...)` / `getConstructData(...)`, current `setPrototype(...)`, current `removeDirect(...)` / `putDirect(...)`, and explicit `QString` / `UString` bridges
- revalidated the direct MSVC `CsScript` probe and reduced the observed failure volume from `162` errors to `78` errors
- moved the dominant first-failure hotspot away from `qscriptengine.cpp` / `qscriptvalue.cpp` and into the next reduced bridge frontier centered on `qscriptqobject.cpp`
- documented the new engine/value contraction pass and its validation evidence

## 0.1.5 - 2026-04-05
- re-established detached persisted-log `CsScript` probing in `build-vs2019-script-probe5`
- captured the next reduced first-failure frontier beyond the declarative/class delegate ABI pass in `src/script/api/qscriptengine.cpp`
- documented the new clustered engine-layer drift around translation helpers, error creation, engine bootstrap signatures, `WriteBarrier` access, native-frame metadata, GC marking, and debugger/evaluation helpers
- preserved all existing background processes while launching a new detached probe wrapper for deeper diagnostic capture

## 0.1.4 - 2026-04-05
- adapted restored declarative/class delegate call and construct plumbing to the current JavaScriptCore host-call ABI
- replaced stale enum-style `JSC::TypeError` throw assumptions in the reduced declarative/class frontier with `createTypeError(...)` + `throwError(...)`
- revalidated `build-vs2019-script-probe5/src/script/CsScript.vcxproj` and confirmed the prior reduced frontier in `qscriptdeclarativeobject.cpp` / `qscriptclassobject.cpp` no longer appears first
- observed the direct MSVC probe progress materially deeper into later JavaScriptCore/API compilation before timing out
- added detached probe5 persisted-log helper scaffolding and documented the new deeper validation state

## 0.1.3 - 2026-04-05
- added a narrow JavaScriptCore compatibility bridge for restored QtScript-era host functions:
  - `runtime/NativeFunctionWrapper.h`
  - `runtime/PrototypeFunction.h`
  - `JSGlobalObject::prototypeFunctionStructure()` compatibility accessor
- updated restored Script wrapper/global-object code to target the current host-call ABI and current `WriteBarrier` conventions
- revalidated direct MSVC Script bring-up with `build-vs2019-script-probe5/src/script/CsScript.vcxproj`
- removed the prior missing `PrototypeFunction` / `NativeFunctionWrapper` frontier and reduced the next fatal cluster to legacy declarative bridge call plumbing, especially `qscriptdeclarativeobject.cpp`
- documented the host-function compatibility pass and its new reduced frontier

## 0.1.2 - 2026-04-05
- captured the first post-linkage-unblock fatal `CsScript` diagnostics from a detached persisted-log MSVC build
- identified the next reduced recovery frontier in:
  - `src/script/api/qscriptcontext.cpp`
  - `src/script/api/qscriptcontextinfo.cpp`
  - `src/script/bridge/qscriptfunction_p.h`
- began the next adaptation pass in `qscriptcontext.cpp` and `qscriptcontextinfo.cpp` by contracting several obvious exception/error/scope-chain and metadata-signature mismatches
- documented the new context-layer recovery frontier and persisted-log validation strategy

## 0.1.1 - 2026-04-05
- advanced the restored `CsScript` bridge/object layer around the current JavaScriptCore object model
- migrated key restored Script wrapper structure plumbing from stale `PassRefPtr`/`RefPtr` assumptions toward raw `JSC::Structure *` contracts where appropriate
- updated restored Script object wrappers to current `visitChildren(...)` / `OverridesVisitChildren` semantics
- rebuilt restored `JSVariableObject` subclasses (`QScriptActivationObject`, `QScriptStaticScopeObject`) around current symbol-table/register ownership expectations
- enabled `BUILDING_JavaScriptCore` and `BUILDING_WTF` for the recovered `CsScript` target, removing the earlier `JSString.cpp` import/export linkage blocker
- reconfigured and revalidated the direct MSVC `CsScript` project build, which now progresses substantially deeper into JavaScriptCore runtime/JIT compilation before the validation timeout

## 0.1.0 - 2026-04-05
- bootstrap project-local version/changelog tracking for ongoing BTK recovery work
- advanced `CsScript` Stage A by adapting current identifier-table handling to `wtfThreadData()`
- replaced obsolete JSC debugger load/unload assumptions with Script-agent routing
- reworked Script source-provider feedback away from inaccessible `UStringSourceProvider` construction
- modernized multiple Script string/value conversion sites around current `UString`, `jsNumber`, `jsString`, and `constructDate` contracts
- contracted major `qscriptdeclarativeclass.cpp` drift and propagated low-risk `QString`/identifier fixes into nearby Script bridge/API files
- revalidated the direct `CsScript` MSVC build probe and documented the newly exposed bridge-header / linkage blocker frontier
