# BML Buildability Guard — 2026-04-05

## Implemented change
Added a top-level configure-time guard around the optional `Declarative` component.

## Behavior
If a user enables `WITH_DECLARATIVE`, BTK now checks for key prerequisites of the legacy declarative runtime.

When they are missing, BTK:
- emits a clear warning
- explains that the legacy declarative runtime still depends on unavailable QtScript/CsScript-era infrastructure
- forces `WITH_DECLARATIVE` back to `OFF`

## Why this is useful
Before this guard, BML/declarative work could appear deceptively close to buildable just because the source tree exists.

The guard makes the truth explicit:
- the naming/bootstrap layer exists
- the underlying runtime still has hard missing dependencies

## Checked blockers
The guard currently checks for indicators including:
- missing `src/script/CMakeLists.txt`
- missing QtScript public headers such as `qscriptengine.h`
- missing `qscriptdeclarativeclass_p.h` support infrastructure

## Result
This keeps default BTK configuration stable while making the current BML buildability gap visible and actionable.
