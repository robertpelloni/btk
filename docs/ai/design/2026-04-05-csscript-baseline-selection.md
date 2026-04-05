# CsScript Baseline Selection — 2026-04-05

## Decision
Select the following historical recovery baselines:

### Primary `CsScript` recovery baseline
- **`85fd29b0d114fccb7b8eca371614bbb813ac0a01`**
- This is the parent of `4fb8e3486b0c63cefaeeb2e734a6b6b7762b3170` (`Remove CsScript library from all build files`)

### Deferred `CsScriptTools` recovery baseline
- **`9845b18dcafa2a4552f394a85e70c43f18a8b797`**
- This is the parent of `60e2952b94841a5274ec67caeae56f12a4ff5a89` (`Remove CsScriptTools source`)

## Why `85fd29...` is the right Script baseline
This commit is the last known snapshot that still contains:
- full `src/script/` sources
- `src/script/CMakeLists.txt`
- `src/script/api/api.cmake`
- `src/script/bridge/bridge.cmake`
- `src/script/parser/parser.cmake`

That makes it the best recovery base because it preserves both:
1. the code itself
2. the project's own latest Script-era build wiring

In other words, it is the most recent **complete** Script snapshot, not merely the most recent source snapshot.

## Why not use `9a48596...^` as the main Script baseline?
The parent of the source-removal commit still has the Script sources, but by that point the Script build files were already deleted by `4fb8e348...`.

Using `9a48596...^` would force BTK to combine:
- recent source state
- older build wiring from elsewhere

That is possible, but less clean than starting from `85fd29...`, which already gives a self-consistent source + build baseline.

## Why `CsScriptTools` gets a different baseline
`src/scripttools/` was removed earlier than `src/script/`.
So the last complete ScriptTools snapshot is not aligned with the last complete Script snapshot.

That is acceptable because ScriptTools is currently **deferred** for BML revival.
The immediate BML need is `CsScript`, not debugger/UI tooling.

## Strategic implication
This means BTK now has a clear recovery posture:
- restore `CsScript` from `85fd29...`
- defer `CsScriptTools` recovery unless compiler/runtime needs justify it sooner

This is preferable to trying to restore both subsystems simultaneously.

## Compatibility-first interpretation
Choosing the last complete Script snapshot fits the broader BTK strategy already established elsewhere in the project:
- prefer staged recovery over disruptive reinvention
- prefer internally proven project history over speculative reimplementation
- restore enough substrate for dependent systems first, then expand coverage

For BML, that means:
1. restore Script substrate
2. restore declarative buildability
3. only then broaden ecosystem/runtime coverage
