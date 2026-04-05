# CsScript `qscriptengine.cpp` Frontier - Diagnostic Capture

## Summary
A detached persisted-log `CsScript` probe was re-established in `build-vs2019-script-probe5` after the declarative/class call-ABI contraction pass.

## Process preservation
No existing background processes were terminated.
A new detached wrapper was launched instead of interfering with existing synchronous or detached work.

## Detached probe details
Probe tree:
- `build-vs2019-script-probe5`

Detached launcher used:
- `build-vs2019-script-probe5/run_csscript_background_wmi.ps1`

Detached wrapper PID:
- `119160`

Persisted logs:
- `build-vs2019-script-probe5/csscript-direct-background.out.log`
- `build-vs2019-script-probe5/csscript-direct-background.err.log`

## First reduced fatal diagnostics captured
The first reduced fatal frontier observed from the persisted log is now concentrated in:
- `src/script/api/qscriptengine.cpp`

Representative diagnostics captured from the log include:
- `error C2039: 'UTF8String': is not a member of 'QTJSC::UString'`
- `error C2039: 'GeneralError': is not a member of 'QTJSC'`
- `error C2039: 'TypeError': is not a member of 'QTJSC'`
- `error C2661: 'QTJSC::jsString': no overloaded function takes 1 arguments`
- `error C2660: 'QTJSC::JSGlobalData::create': function does not take 0 arguments`
- `error C2660: 'QScriptObject::createStructure': function does not take 1 arguments`
- `error C2661: 'QScript::QObjectPrototype::QObjectPrototype': no overloaded function takes 2 arguments`
- `error C2440: 'return': cannot convert from 'QTJSC::WriteBarrier<QTJSC::JSObject>' to 'QTJSC::JSObject *'`
- `error C2039: 'thisValue': is not a member of 'QTJSC::ExecState'`
- `error C2039: 'returnValueRegister': is not a member of 'QTJSC::ExecState'`
- `error C2039: 'ReturnValueRegister': is not a member of 'QTJSC::RegisterFile'`
- multiple `MarkStack::append(...)` failures caused by raw pointer / `JSValue` usage instead of current barrier-aware appends
- `error C2039: 'evaluateStart': is not a member of 'QTJSC::Debugger'`
- `error C2664` for the old `DynamicGlobalObjectScope` construction form

## Interpretation
This is a meaningful forward movement.
The prior reduced frontier in restored declarative/class delegate call plumbing was successfully contracted, and the next first-failure hotspot is now a single, high-leverage engine-layer file.

## Immediate recommendation
Attack `qscriptengine.cpp` as a clustered adaptation pass in the following order:
1. translation helper conversions and error creation
2. engine bootstrap signatures
3. global-object / barrier accessors
4. native context flag storage assumptions
5. GC mark-stack barrier appends
6. debugger/evaluation helper drift
