# CHANGELOG

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
