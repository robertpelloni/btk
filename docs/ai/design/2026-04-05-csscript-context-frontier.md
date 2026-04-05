# CsScript Context Frontier — 2026-04-05

## Summary
After the restored `CsScript` bridge/object layer and the embedded-JavaScriptCore export-definition mismatch were both contracted, the next reduced failure frontier is now centered in the Script context layer.

## Newly exposed frontier
### 1. Script context API drift
`src/script/api/qscriptcontext.cpp` still encodes older assumptions about:
- exception setting on `ExecState`
- old enum-style error dispatch
- host/native call-frame argument storage helpers
- scope-chain copying
- raw-pointer access to `WriteBarrier`-owned scope nodes/objects
- return-register layout details

### 2. Script context-info metadata drift
`src/script/api/qscriptcontextinfo.cpp` still encodes older assumptions about:
- codeblock line-number helper signatures
- `info` vs `s_info` metadata symbols
- per-parameter-name access on `FunctionExecutable`

### 3. Legacy function-wrapper substrate gap
`src/script/bridge/qscriptfunction_p.h` still depends on a historical `PrototypeFunction` layer that is no longer directly available in the current embedded tree.

## Architectural reading
This is a healthier frontier than the previous one.

Why:
- it is concentrated in a small number of files
- it is closer to Script’s public API surface
- it follows successful contraction of deeper bridge/object and linkage/export layers

That means the recovery effort is now climbing upward from substrate survival toward usable Script API behavior.

## Current implication for BML
This is still upstream of BML runtime revival, but it is a better upstream position than before.

The recovered Script layer is no longer primarily fighting for object-model survival.
It is now fighting over context semantics and wrapper affordances, which is a more promising place to be if the longer-term goal is reviving the legacy declarative/BML stack.

## Recommended next patch order
1. `qscriptcontext.cpp`
2. `qscriptcontextinfo.cpp`
3. `qscriptfunction_p.h/.cpp`

Then rerun the persisted-log build capture again if needed.
