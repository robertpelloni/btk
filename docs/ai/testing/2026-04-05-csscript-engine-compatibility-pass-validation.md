# CsScript Engine Compatibility Pass Validation — 2026-04-05

## Goal
Validate that the latest `CsScript` compatibility pass materially reduces the old JavaScriptCore engine-glue failure cluster and exposes a narrower next frontier.

## Process preservation
Per workspace instructions, no background processes were terminated.
A fresh process audit was performed with:
- `tasklist`

## Validation commands
Primary direct Script build probe:
- `powershell.exe -NoProfile -Command "& 'C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe' 'C:\Users\hyper\workspace\btk\build-vs2019-script-probe3\src\script\CsScript.vcxproj' /p:Configuration=Release /p:BuildProjectReferences=false /m:1 /nologo /v:minimal; Write-Output \"EXITCODE:$LASTEXITCODE\""`

## What was validated

### 1. The build still reaches direct `CsScript` compilation
Validation confirmed the direct project build still reaches Script-specific compilation rather than failing at configure time or on missing JavaScriptCore paths.

### 2. The old engine-glue failures were materially contracted
The newest pass removed or reduced the earlier failure categories around:
- legacy identifier-table helpers (`currentIdentifierTable` / `setCurrentIdentifierTable`)
- obsolete debugger-base `scriptLoad` / `scriptUnload` assumptions
- inaccessible `UStringSourceProvider` constructor use
- multiple `qscriptdeclarativeclass.cpp` failures caused by:
  - `JSValue(exec, number)` construction
  - `jsString(exec, QString)`
  - `UString::Rep`
  - `.rep()` / scope-chain pointer assumptions

### 3. The build frontier shifted to deeper bridge/header drift
After the pass, the dominant remaining failures are now concentrated in:
- `src/script/bridge/qscriptobject_p.h`
- `src/script/bridge/qscriptstaticscopeobject_p.h`
- `src/script/bridge/qscriptqobject_p.h`
- `src/script/bridge/qscriptactivationobject_p.h`

Representative current failures now include:
- `OverridesMarkChildren` no longer present under the old contract
- `Structure::create(...)` signature drift
- `JSVariableObjectData` removed / no longer available under the historical shape
- `JSVariableObject::d` no longer present
- `JSVariableObject::isDynamicScope(bool &) const` now required by the current base type

### 4. A separate JavaScriptCore linkage issue is now visible
The direct build also surfaces a parallel blocker in the current target arrangement:
- `runtime/JSString.cpp` reports `dllimport static data member` / `s_info` linkage mismatches

This appears distinct from Script bridge-header drift and should be treated as a separate target/export visibility investigation.

## Validation reading
This validation is a success for Stage A recovery even though the module still does not build fully.

Why:
- the build got meaningfully farther
- the error stream is now cleaner
- the remaining blockers are better clustered
- the next adaptation targets are now clearer and more local

## Current blocker summary

### Primary blocker cluster
Restored Script bridge classes are still shaped against an older JavaScriptCore variable-object and structure API.

Most urgent files:
- `src/script/bridge/qscriptobject_p.h`
- `src/script/bridge/qscriptstaticscopeobject_p.h/.cpp`
- `src/script/bridge/qscriptactivationobject_p.h/.cpp`
- `src/script/bridge/qscriptqobject_p.h`

### Secondary blocker cluster
Current `CsScript` target/export plumbing still appears to have a JavaScriptCore linkage mismatch around `JSString.cpp` static metadata symbols.

## Recommended next validation step
After the next bridge-header modernization pass, rerun the same direct project build command so the error delta remains comparable across passes.
