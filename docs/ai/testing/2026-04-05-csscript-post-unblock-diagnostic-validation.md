# CsScript Post-Unblock Diagnostic Validation — 2026-04-05

## Goal
Validate that the first fatal diagnostics after the bridge/object and linkage-export unblocks were successfully captured from a longer-running direct MSVC `CsScript` build.

## Process preservation
No background processes were terminated.
A fresh process audit was performed with:
- `tasklist`

## Validation method
A detached background build was launched so the build could continue writing logs even after the immediate command returned.

The build logs were then inspected directly from disk.

## Background process captured
Detached wrapper process observed:
- PID `120812`
- `cmd.exe`

Observed command line wrapped a direct build of:
- `build-vs2019-script-probe3/src/script/CsScript.vcxproj`

## Log files validated
- `build-vs2019-script-probe3/csscript-direct-background.out.log`
- `build-vs2019-script-probe3/csscript-direct-background.err.log`

## Validation result
The first post-unblock fatal diagnostics were successfully captured.

### Primary files now exposed as the next frontier
- `src/script/api/qscriptcontext.cpp`
- `src/script/api/qscriptcontextinfo.cpp`
- `src/script/bridge/qscriptfunction_p.h`

### Confirmed failure categories
- old exception-setting contract
- old enum-style error-construction contract
- old native call-frame argument helpers
- old scope-chain copying assumptions
- old `WriteBarrier` to raw-pointer assumptions
- old `RegisterFile::ReturnValueRegister` assumption
- old codeblock line-number helper shape
- old `info` vs current `s_info` symbol assumptions
- missing `PrototypeFunction.h`

## Validation reading
This validation is successful because it answers the most important next question from the prior pass:

> what is the first unreduced fatal blocker after the restored Script bridge/object layer and `JSString.cpp` linkage problem were contracted?

We now have a clear answer, and it is more localized than the previous frontier.

## Recommended next action
Proceed with the newly started adaptation pass in:
- `qscriptcontext.cpp`
- `qscriptcontextinfo.cpp`
- `qscriptfunction_p.h/.cpp`

and then rerun the same persisted-log capture pattern if the next direct build again outruns synchronous tool windows.
