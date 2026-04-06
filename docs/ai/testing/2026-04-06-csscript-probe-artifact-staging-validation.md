# CsScript Probe Artifact Staging - Validation

## Objective
Validate that the currently missing BTK core artifacts can be staged into the isolated Script probe tree at the exact paths expected by `CsScript.vcxproj`.

## Environment
- Main validated build tree: `build-vs2019`
- Isolated Script probe tree: `build-vs2019-script-probe5`
- Configuration: `Release`

## Safety
- Fresh process audits performed.
- No active background processes were terminated.
- Existing detached linked-build investigation left running.

## Validation evidence
### Source artifact existence in the validated main build
Confirmed present in the validated main tree:
- `build-vs2019/lib/Release/CsCore2.1.lib`
- `build-vs2019/bin/Release/CsCore2.1.dll`

### Probe-local destination expectation
Confirmed from project inspection that `CsScript.vcxproj` expects:
- `..\\..\\lib\\Release\\CsCore2.1.lib`

which resolves to:
- `build-vs2019-script-probe5/lib/Release/CsCore2.1.lib`

### Staging result
Confirmed present after staging:
- `build-vs2019-script-probe5/lib/Release/CsCore2.1.lib`
- `build-vs2019-script-probe5/bin/Release/CsCore2.1.dll`

## Interpretation
This validation confirms that the previously missing probe-local BTK core artifacts are now materialized at the paths expected by the isolated Script link step.

It does **not** yet prove end-to-end linked success for `CsScript`, because:
- the detached linked build was already active and still being observed,
- its logs had not yet populated during this session,
- and the next linked frontier beyond the staged core artifacts still needs to be captured.

## Recommended next step
Continue polling:
- the active detached linked-build processes,
- `csscript-with-refs-background.out.log`
- `csscript-with-refs-background.err.log`
- the probe-local `bin/Release` and `lib/Release` outputs

to determine whether the staged artifacts allow the current linked run to progress to the next reduced frontier.