# CsScript Probe Artifact Staging - Implementation

## Summary
Added a reusable staging helper and used it to materialize the currently missing `CsCore` artifacts into the isolated Script probe tree without terminating any active processes.

## Safety / process preservation
- Fresh process audits were performed before acting.
- No active background processes were terminated.
- The existing detached linked-build investigation remained running.

## Implementation
### 1. Added reusable staging helper
New script:
- `scripts/stage_probe_core_artifacts.ps1`

Purpose:
- copy `CsCore2.1.lib` and `CsCore2.1.dll` from the validated main build tree into the matching probe-local locations expected by `build-vs2019-script-probe5`
- create the destination `lib/<config>` and `bin/<config>` directories if needed
- avoid overwriting the destination files if they already exist

Default roots:
- main build: `build-vs2019`
- probe build: `build-vs2019-script-probe5`
- configuration: `Release`

### 2. Staged the current missing core artifacts
Used the validated main build outputs:
- `build-vs2019/lib/Release/CsCore2.1.lib`
- `build-vs2019/bin/Release/CsCore2.1.dll`

Staged to the isolated probe locations:
- `build-vs2019-script-probe5/lib/Release/CsCore2.1.lib`
- `build-vs2019-script-probe5/bin/Release/CsCore2.1.dll`

### 3. Preserved the active detached linked build
Did not kill or restart the existing detached linked-build investigation.
The active detached wrapper and observed worker processes were left running so they can continue and potentially consume the newly staged artifacts.

## Interpretation
This pass does not claim that the probe tree has now independently built all prerequisites.
It truthfully narrows the current frontier by placing the exact missing BTK core artifacts at the explicit locations required by `CsScript.vcxproj`.

That means the next linked failure, if any, should be more informative than the previous missing-`CsCore2.1.lib` stop.