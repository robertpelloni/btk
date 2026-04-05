# CsScript Bridge Structure Pass Validation — 2026-04-05

## Goal
Validate that the latest Stage A `CsScript` bridge/object modernization pass and target-definition fix move the direct MSVC build materially beyond the previous bridge/header and `JSString.cpp` linkage blockers.

## Process preservation
No background processes were terminated.
A fresh process audit was performed with:
- `tasklist`

## Validation commands
### Reconfigure experimental Script probe
- `cmake -S . -B build-vs2019-script-probe3 -G "Visual Studio 16 2019" -A x64 -DWITH_SCRIPT=ON`

### Direct Script build probe
- `powershell.exe -NoProfile -Command "& 'C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe' 'C:\Users\hyper\workspace\btk\build-vs2019-script-probe3\src\script\CsScript.vcxproj' /p:Configuration=Release /p:BuildProjectReferences=false /m:1 /nologo /v:minimal; Write-Output \"EXITCODE:$LASTEXITCODE\""`

## Validation results

### 1. Reconfigure still succeeds with experimental Script enabled
The probe configure still succeeds with:
- `WITH_SCRIPT=ON`
- `Script` listed in the BTK library set

This confirms the latest source/CMake edits preserved experimental configure viability.

### 2. The prior `JSString.cpp` export/linkage blocker no longer appeared
In the previous pass, the direct build exposed a distinct blocker around:
- `runtime/JSString.cpp`
- `dllimport` / `s_info` static metadata mismatch

After adding:
- `BUILDING_JavaScriptCore=1`
- `BUILDING_WTF=1`

to the recovered Script target definitions, that earlier fatal linkage/export failure did not reappear in the later timed build probes.

### 3. The build now progresses substantially deeper into JavaScriptCore compilation
The direct build continued well past the earlier wrapper/header frontier and compiled significantly deeper translation units, including examples such as:
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

That is materially beyond the earlier failure frontier.

### 4. The newest run timed out while still compiling
The latest validation run hit the tool timeout while compilation was still progressing.

Important reading:
- this is **not** evidence of build success yet
- this is also **not** the earlier immediate failure mode
- it means the current reduced blocker frontier was not reached within the capture window

## Validation interpretation
This pass is a recovery success because it removed or contracted three previously dominant blocker classes:
- Script engine-glue drift
- Script bridge/object-model drift
- `JSString.cpp` export/linkage drift

The next unresolved question is no longer “can the recovered Script layer get past its first private-API failures?”
It now becomes:
- what is the **next** fatal blocker after the build spends substantial time compiling deeper JavaScriptCore runtime/JIT sources?

## Remaining limitation
Because the direct build timed out before completion, this pass does not yet prove:
- a successful `CsScript` build
- or the exact next fatal post-`JSString` blocker

## Recommended next validation step
Run the same direct project build again with:
- a longer timeout, and/or
- a persisted full log capture

Then record the first unreduced fatal diagnostic beyond the currently fixed bridge/header and export-definition layers.
