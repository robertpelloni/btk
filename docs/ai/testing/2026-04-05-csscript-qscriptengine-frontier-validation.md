# CsScript `qscriptengine.cpp` Frontier - Validation

## Objective
Validate that, after the declarative/class delegate call-ABI contraction pass, the next reduced first-failure frontier can be isolated from a detached persisted-log probe.

## Environment
- Host: Windows
- Generator: Visual Studio 16 2019
- Probe tree: `build-vs2019-script-probe5`
- Target: `build-vs2019-script-probe5/src/script/CsScript.vcxproj`
- Script mode: `WITH_SCRIPT=ON`

## Safety
A fresh process audit was performed.
No pre-existing background processes were terminated.

## Detached validation path
A detached persisted-log probe was launched via:
- `build-vs2019-script-probe5/run_csscript_background_wmi.ps1`

Observed wrapper PID:
- `119160`

Observed logs:
- `build-vs2019-script-probe5/csscript-direct-background.out.log`
- `build-vs2019-script-probe5/csscript-direct-background.err.log`

## Result
The detached log successfully captured the next reduced first-failure frontier beyond the earlier declarative/class delegate ABI cluster.

### New first-failure hotspot
- `src/script/api/qscriptengine.cpp`

### Representative captured errors
- removed `UString::UTF8String()` assumptions
- removed `GeneralError` / `TypeError` enum-style dispatch
- `jsString(...)` contract drift
- engine bootstrap signature drift around `JSGlobalData::create()` and prototype construction
- `WriteBarrier` to raw-pointer conversion failures
- removed native-frame helpers like `thisValue()` and `returnValueRegister()` in the expected old form
- barrier-aware `MarkStack::append(...)` drift
- debugger/evaluation helper contract drift

## Interpretation
This validation confirms two things:
1. the previous reduced declarative/class delegate call-ABI frontier was successfully contracted,
2. the next reduced compiler frontier is now a concentrated engine-layer hotspot rather than a broad unresolved surface.

## Recommended next validation step
After a focused `qscriptengine.cpp` adaptation pass, rerun the same detached persisted-log probe pattern and capture the next reduced first-failure frontier.
