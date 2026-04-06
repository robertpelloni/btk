# CsScript Probe Artifact Staging

## Context
The isolated `build-vs2019-script-probe5` tree has now crossed an important threshold:
- source compilation for `CsScript` succeeds,
- but full target validation is blocked by probe-local dependency materialization.

The immediate linked failure is:
- `LINK : fatal error LNK1181: cannot open input file '..\\..\\lib\\Release\\CsCore2.1.lib'`

## Design question
How should we treat a probe tree that is intentionally isolated, but whose direct target build depends on already-validated BTK core artifacts?

## Chosen approach
Use **artifact staging** as an explicit bridge between:
- the already-validated main build tree (`build-vs2019`), and
- the experimental Script probe tree (`build-vs2019-script-probe5`).

This is truthful because:
- the main tree already contains empirically built `CsCore2.1.lib` / `CsCore2.1.dll` artifacts,
- the probe tree's project files explicitly expect those exact artifacts at probe-local conventional paths,
- staging them does not claim that the probe has independently built all prerequisites,
- it simply removes one materialization boundary so the next linked Script failure can be observed.

## Why artifact staging is preferable here
Compared with immediately forcing more synchronous dependency rebuilds in the same probe tree, staging has several benefits:
1. it does not require killing or interrupting any active background builds,
2. it preserves the compile-only milestone already reached,
3. it uses already-known-good BTK artifacts from a validated tree,
4. it can expose the next truthful linked frontier faster than waiting on a potentially long dependency rebuild cycle.

## Scope
This pass stages only the currently missing BTK core artifacts required by the Script link step:
- `CsCore2.1.lib`
- `CsCore2.1.dll`

It does **not** claim that all downstream linked/runtime prerequisites are now satisfied.
It is a targeted frontier-contraction step.

## Expected outcome
Once the missing core artifacts are present in the probe-local locations, one of two things should happen:
1. the active or next `CsScript` linked build progresses beyond the current missing-library stop and exposes the next linked frontier, or
2. another probe-local dependency gap appears, which can then be documented just as precisely.

Either outcome is useful because it sharpens the next Stage A truth boundary.