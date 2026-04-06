# CsScript Link-Materialization Investigation - Implementation

## Summary
Followed the post-compile recommendation to investigate why the isolated `CsScript` probe still fails at link after source compilation now succeeds.

## Safety / process preservation
- Performed fresh process audits before additional validation work.
- Did not terminate any existing background processes.
- Left all previously running detached/background work untouched.

## Investigation actions

### 1. Inspected the isolated probe layout
Reviewed `build-vs2019-script-probe5` and confirmed:
- the probe tree has conventional `bin/` and `lib/` directories,
- but no `CsCore2.1.lib` was currently present under the probe-local `lib/Release/` path,
- and no `CsCore` import library was otherwise materialized anywhere in the probe tree at inspection time.

### 2. Inspected `CsScript.vcxproj`
Reviewed:
- `build-vs2019-script-probe5/src/script/CsScript.vcxproj`

Confirmed that the Script target:
- explicitly links `..\\..\\lib\\Release\\CsCore2.1.lib`,
- retains a `ProjectReference` to `src/core/CsCore.vcxproj`,
- but also sets `LinkLibraryDependencies=false`.

Interpretation:
- the Script link step expects `CsCore2.1.lib` to exist as a materialized probe-local artifact,
- rather than depending on automatic transitive link propagation from the project reference.

### 3. Inspected `CsCore.vcxproj`
Reviewed:
- `build-vs2019-script-probe5/src/core/CsCore.vcxproj`

Confirmed that `CsCore` is configured to emit its import library to:
- `build-vs2019-script-probe5/lib/Release/CsCore2.1.lib`

This validates that the missing library path is not arbitrary or stale: it is the intended output location for the referenced dependent target.

### 4. Revalidated current source-vs-link state
Reconfirmed:
- `MSBuild /t:ClCompile` for `CsScript` succeeds with `0` errors,
- full direct `CsScript` build with project references disabled still fails at the missing import-library link step.

### 5. Re-established detached persisted-log investigation for the linked path
Added helper:
- `build-vs2019-script-probe5/run_csscript_with_refs_background_wmi.ps1`

This helper launches a detached linked Script build with:
- `BuildProjectReferences=true`

It writes to:
- `build-vs2019-script-probe5/csscript-with-refs-background.out.log`
- `build-vs2019-script-probe5/csscript-with-refs-background.err.log`

### 6. Launched the new detached linked build
Launched the helper and observed:
- wrapper PID: `149944`
- active child processes observed after launch included:
  - `MSBuild` PID `155824`
  - `cl` PID `98360`

At observation time:
- the detached processes were still active,
- the new linked-build logs had not yet populated,
- `CsCore2.1.lib` was still not yet materialized in the probe tree.

## Interpretation
This investigation narrows the next frontier further:
- compile-only recovery is real and preserved,
- the isolated full-target frontier is now centered on dependency build/materialization timing and/or upstream prerequisite completion in the probe,
- a detached linked build is now actively running to capture the truth without killing or disturbing other processes.

## Current state at end of session
- active detached linked-build wrapper remains running by design:
  - PID `149944`
- active observed tool processes during polling included:
  - `MSBuild` PID `155824`
  - `cl` PID `98360`
- new logs to monitor:
  - `build-vs2019-script-probe5/csscript-with-refs-background.out.log`
  - `build-vs2019-script-probe5/csscript-with-refs-background.err.log`
- the next agent should poll those logs and the probe-local `lib/Release/` directory before deciding on the next move.