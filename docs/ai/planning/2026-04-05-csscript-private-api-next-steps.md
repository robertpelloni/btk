# CsScript Private API Next Steps — 2026-04-05

## Goal
Move from first direct `CsScript` compile diagnostics into a disciplined compatibility adaptation sequence.

## Next-pass objective
Do **not** attempt a broad blind rebuild first.
Instead, make one focused pass on the highest-leverage private-API hotspot:
- `src/script/api/qscriptengine_p.h`

## Why start there
`qscriptengine_p.h` concentrates several classes of failure at once:
- identifier-table access
- debugger load/unload hooks
- `UString` conversion helpers
- primitive JS value creation helpers
- exception plumbing

A focused pass there has the best chance of reducing a large amount of downstream error noise.

## Suggested task breakdown
### Step 1 — `qscriptengine_p.h` symbol-contract patch inventory
For each first-failure contract in that file, decide whether it is:
- rename/remap
- signature drift
- visibility drift
- semantic removal

### Step 2 — Patch obvious current-equivalent cases only
Candidates already supported by evidence:
- identifier-table access via `wtfThreadData().setCurrentIdentifierTable(...)`
- some `UString` / string-conversion call-site updates

### Step 3 — Rebuild direct `CsScript.vcxproj`
Use the same direct-project strategy so diagnostics stay Script-focused.

### Step 4 — Re-rank the next hotspot set
Likely next candidates after `qscriptengine_p.h`:
- `qscriptactivationobject*`
- `qscriptstaticscopeobject*`
- `qscriptobject*` / `qscriptqobject*`
- `qscriptdeclarativeclass*`

## Success condition for next pass
The next pass should aim to do one of two things:
1. materially reduce the error count in `qscriptengine_p.h`
2. or prove that a smaller prerequisite compatibility layer is needed before meaningful patching can continue

Either outcome is useful, as long as it is specific and documented.
