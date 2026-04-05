# CsScript `qscriptengine.cpp` Frontier

## Context
After contracting the restored declarative/class delegate host-call ABI frontier, the next detached persisted-log probe moved the first fatal Script errors into `src/script/api/qscriptengine.cpp`.

This is an important transition point:
- the build is no longer stopping first in the smaller declarative/class delegate cluster,
- the next reduced frontier is now the broader Script engine runtime/bootstrap layer,
- this file is a high-leverage hotspot because it touches engine bootstrap, translation helpers, global-object ownership, timeout plumbing, native-frame context metadata, and GC marking.

## Observed frontier categories
The detached `probe5` log shows several recurring mismatch classes in `qscriptengine.cpp`:

### 1. Removed `UString::UTF8String()` assumptions
Recovered code still expects historical conversion helpers like:
- `context.UTF8String().c_str()`
- `text.UTF8String().c_str()`
- `id.UTF8String().c_str()`

Current BTK/WebKit requires going through the established Script-side conversion helpers instead of historical `UTF8String()` access.

### 2. Removed enum-style `throwError(..., GeneralError/TypeError, ...)`
Recovered code still assumes removed enum dispatch like:
- `JSC::GeneralError`
- `JSC::TypeError`

This needs the same modernization pattern already used elsewhere:
- `createError(...)`, `createTypeError(...)`, etc.
- then `throwError(exec, ...)`

### 3. Engine bootstrap/API drift
The engine constructor still expects several historical contracts that drifted:
- `JSGlobalData::create()` no-arg form
- older prototype constructor signatures
- older `createStructure(...)` convenience forms
- timeout checker pointer ownership assumptions

### 4. Global-object / write-barrier drift
Recovered code still treats multiple fields as raw pointers where current JSC stores `WriteBarrier<...>` values.
Representative areas:
- `originalGlobalObject()` / `customGlobalObject()`
- global object swapping
- GC marking

### 5. Native-frame context metadata drift
Recovered code still assumes removed native-frame helpers like:
- `ExecState::thisValue()` in the old context
- `ExecState::returnValueRegister()`
- `RegisterFile::ReturnValueRegister`

These likely need to follow the same compatibility-softening approach already used in nearby context-layer recovery.

### 6. Debugger / dynamic-global scope drift
Recovered code still expects older debugger/runtime contracts, for example:
- `Debugger::evaluateStart(...)`
- old `DynamicGlobalObjectScope` construction form

## Why this frontier matters
`qscriptengine.cpp` is the next major reduction point because it governs:
- engine creation
- global object topology
- translation helper host functions
- exception/evaluation control flow
- object/property bridge helpers
- GC marking roots

Successfully contracting it would move BTK materially closer to an experimentally viable `CsScript` runtime and therefore closer to making legacy declarative/BML bring-up testable again.

## Recommended next pass
Treat this as a clustered frontier, not a one-line fix.
Suggested reduction order:
1. translation helper string/error modernization (`UTF8String`, `GeneralError`, `TypeError`)
2. engine bootstrap constructor/signature drift in the `QScriptEnginePrivate` constructor
3. global-object / `WriteBarrier` accessor cleanup
4. context-flag / native-frame metadata drift
5. GC marking `append(&barrier)` / `.get()` corrections
6. debugger/evaluation helper drift
