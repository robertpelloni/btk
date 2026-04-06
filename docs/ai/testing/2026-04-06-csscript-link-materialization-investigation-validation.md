# CsScript Link-Materialization Investigation - Validation

## Objective
Validate the current post-compile state of restored `CsScript` and determine whether the isolated probe is now blocked by source compilation or by dependent-library materialization.

## Environment
- Host: Windows
- Toolchain: Visual Studio 2019 Build Tools / MSBuild
- Probe tree: `build-vs2019-script-probe5`
- Target projects inspected:
  - `src/script/CsScript.vcxproj`
  - `src/core/CsCore.vcxproj`

## Safety
- Fresh process audits performed.
- No existing background processes were terminated.
- Detached linked-build investigation launched instead of disturbing prior work.

## Validation results

### 1. Compile-only validation remains successful
Observed:
- `MSBuild /t:ClCompile` for `build-vs2019-script-probe5/src/script/CsScript.vcxproj`
- result: `Build succeeded.` with `0 Error(s)`

### 2. Full isolated Script build with project references disabled still fails at link
Observed failure:
- `LINK : fatal error LNK1181: cannot open input file '..\\..\\lib\\Release\\CsCore2.1.lib'`

Interpretation:
- the current blocker is not the previously dominant source frontier,
- it is the absence of the expected `CsCore` import library in the probe tree.

### 3. Project-file validation confirms the expected output path
Observed from project inspection:
- `CsScript.vcxproj` explicitly links `..\\..\\lib\\Release\\CsCore2.1.lib`
- `CsCore.vcxproj` is configured to emit its import library to `build-vs2019-script-probe5/lib/Release/CsCore2.1.lib`

Interpretation:
- the missing file path is consistent across producer and consumer,
- so the current truth is a materialization/build-order/probe-completeness issue, not an obviously wrong link path.

### 4. Detached linked-build investigation launched successfully
Helper added and executed:
- `build-vs2019-script-probe5/run_csscript_with_refs_background_wmi.ps1`

Observed launch result:
- wrapper PID `149944`

Observed active child work after launch:
- `MSBuild` PID `155824`
- `cl` PID `98360`

Observed log targets:
- `build-vs2019-script-probe5/csscript-with-refs-background.out.log`
- `build-vs2019-script-probe5/csscript-with-refs-background.err.log`

At polling time:
- both logs were still zero-length,
- `CsCore2.1.lib` was not yet present,
- the detached linked build was still active.

## Conclusion
This validation confirms the new truthful frontier:
- source compilation for the reduced `CsScript` target is now materially successful,
- the isolated next frontier is dependent-library materialization for linked validation,
- a detached linked build is now running to capture whether that materialization completes and what the next linked failure is if it does not.

## Recommended next validation step
Poll the active detached linked build's logs and probe-local `lib/Release/` output before making further source changes.
If `CsCore2.1.lib` appears, rerun or observe the next linked failure beyond the current materialization boundary.