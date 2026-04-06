# CsScript QObject Bridge Compilation Pass - Validation

## Objective
Validate that the reduced QObject bridge frontier can be contracted far enough for restored `CsScript` to complete source compilation in the isolated MSVC probe tree.

## Environment
- Host: Windows
- Toolchain: Visual Studio 2019 Build Tools / MSBuild
- Probe tree: `build-vs2019-script-probe5`
- Target: `build-vs2019-script-probe5/src/script/CsScript.vcxproj`
- Script mode: `WITH_SCRIPT=ON`

## Safety
- Fresh process audits performed.
- No existing background processes were terminated.
- Existing detached processes were left undisturbed.

## Validation steps
### 1. Direct target build
Ran a direct MSVC target build with project references disabled to keep the probe isolated while surfacing the next frontier honestly.

Observed result after the QObject bridge/source contractions:
- the build no longer stops in the earlier `qscriptqobject.cpp` source-failure wall,
- it now reaches link stage,
- current isolated-probe stop is:
  - `LINK : fatal error LNK1181: cannot open input file '..\..\lib\Release\CsCore2.1.lib'`

### 2. Compile-only validation
Ran:
- `MSBuild /t:ClCompile`

Observed result:
- `Build succeeded.`
- `0 Warning(s)`
- `0 Error(s)`

## Result
This validation confirms that the reduced Stage A source frontier has moved again:
- previous dominant source frontier: `src/script/bridge/qscriptqobject.cpp`
- current state: compile-only success for the isolated `CsScript` target
- next isolated frontier: link/dependency materialization in `build-vs2019-script-probe5`

## Evidence of source-frontier contraction
The pass specifically removed/contracted failures around:
- QObject bridge host-call ABI mismatches
- enum-style error dispatch (`TypeError`, `GeneralError`, `SyntaxError`)
- `Heap::isCellMarked(...)` drift
- `MarkStack::append(...)` access/root handling drift
- `getCallData(...)` helper drift
- `setPrototype(...)` signature drift
- `QString` / `UString` conversion breakage across the reduced bridge surface
- final residual `UString` assignment drift in `qscriptcontextinfo.cpp`
- final residual `formatArg(...)` string drift in `qscriptengine.cpp`

## Interpretation
This is another real forward movement milestone.
`CsScript` is not fully revived yet, but the source adaptation frontier has progressed from engine/value failures, through QObject bridge failures, and now into a link-materialization boundary in the isolated probe.

## Recommended next validation step
Investigate the current link boundary truthfully, for example by:
1. determining whether `build-vs2019-script-probe5` should materialize the dependent BTK libraries locally,
2. or rerunning a linked validation path with project references enabled / prerequisites built,
3. while preserving the existing isolated probe evidence and without disturbing existing background processes.