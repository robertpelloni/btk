# CsScript Engine/Value Contraction Pass - Validation

## Objective
Validate that the focused engine/value recovery pass materially contracts the current `CsScript` compiler frontier beyond the earlier `qscriptengine.cpp` wall.

## Environment
- Host: Windows
- Generator/toolchain family: Visual Studio 2019 Build Tools / MSBuild
- Probe tree: `build-vs2019-script-probe5`
- Target: `build-vs2019-script-probe5/src/script/CsScript.vcxproj`
- Script mode: `WITH_SCRIPT=ON`
- Build style: direct target build with project references disabled

## Safety
- Fresh process audit performed before validation.
- No existing background processes were terminated.
- Existing detached probe helpers/processes were left untouched.

## Validation command
Direct MSBuild invocation against:
- `C:\Users\hyper\workspace\btk\build-vs2019-script-probe5\src\script\CsScript.vcxproj`

Key settings:
- `/p:Configuration=Release`
- `/p:Platform=x64`
- `/p:BuildProjectReferences=false`
- `/m:1`
- `/v:minimal`
- `/clp:ErrorsOnly;Summary`

## Result
Validation succeeded in the intended sense: the compiler progressed substantially past the previous engine/value wall and exposed a smaller next frontier.

### Quantitative movement
- previous direct probe snapshot: `162` errors
- current direct probe snapshot after this pass: `78` errors

### What no longer appears as the dominant first frontier
The earlier dominant `qscriptengine.cpp` / `qscriptvalue.cpp` mismatch cluster is no longer the primary hard stop.
The pass successfully contracted those areas enough for the build to move deeper.

### New dominant hotspot
The current first-failure hotspot is now primarily:
- `src/script/bridge/qscriptqobject.cpp`

### Representative remaining diagnostics
- `error C2039: 'isCellMarked': is not a member of 'QTJSC::Heap'`
- `error C2664: 'void QTJSC::MarkStack::append(QTJSC::ConservativeRoots &)'`
- `error C2440: '=': cannot convert ... to 'QTJSC::NativeFunction'`
- `error C2039: 'TypeError': is not a member of 'QTJSC'`
- `error C2039: 'GeneralError': is not a member of 'QTJSC'`
- `error C2039: 'info': is not a member of 'QTJSC::InternalFunction'`
- `error C2039: 'getCallData': is not a member of 'QTJSC::JSValue'`
- `error C2660: 'QTJSC::JSObject::setPrototype': function does not take 1 arguments`

### Residual non-bridge template/conversion noise
Two additional template-heavy include-surface errors remain visible during the direct probe:
- `include/QtCore/cs_string.h(942,1)`
- `include/QtCore/qstringparser.h(271,1)`

These are now trailing conversion/template symptoms rather than the primary Stage A frontier.

## Interpretation
This validation confirms that the latest work was a genuine contraction pass:
1. the recovered Script engine/value layer compiles materially further than before,
2. the first-failure hotspot has shifted into the QObject bridge layer,
3. the total visible failure count has been cut by more than half relative to the previous direct snapshot.

## Recommended next step
Attack `src/script/bridge/qscriptqobject.cpp` as the new reduced clustered frontier, prioritizing:
1. enum-style error creation replacement (`TypeError`, `GeneralError`, `SyntaxError`)
2. `info` → `s_info` metadata updates
3. `QString` / `UString` conversions through `QScript::convertToString(...)` / `toUString(...)`
4. `getCallData(...)` / host-call ABI modernization
5. `setPrototype(...)` signature updates
6. GC/mark-stack barrier-aware root handling replacements