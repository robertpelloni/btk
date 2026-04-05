# CsScript Clustered Drift Plan — 2026-04-05

## Goal
Use the new JavaScriptCore drift audit to drive the next Stage A recovery steps in a structured order.

## Input facts
From the generated drift audit:
- 464 historical Script JavaScriptCore references were detected
- 408 already exist in the current tree
- 56 are missing
- missing references cluster heavily by subtree rather than being evenly distributed

## Recovery strategy
### Phase A1 — Preserve the compatible majority
Objective:
- keep the restored Script baseline anchored to the 408 already-existing references
- avoid unnecessary churn on already-aligned parts of the module

### Phase A2 — Triage missing clusters
Handle missing clusters in this order:
1. `runtime`
2. `pcre`
3. `wrec`
4. `yarr`
5. `generated`
6. `wtf` / `jit` / `unicode`

Rationale:
- `runtime` is the largest and most semantically important drift group
- `pcre` and `wrec` look like clear legacy subsystem deltas
- smaller groups should be addressed after the major architecture drifts are understood

## Tactical questions per cluster
### Runtime cluster
For each missing runtime file, determine:
- was it renamed?
- was it removed upstream?
- was it conceptually replaced by newer classes/files?
- can Script CMake stop referencing it entirely?

### PCRE cluster
Determine whether the historical Script module truly still needs the old `pcre` inputs, or whether current WebKit JavaScriptCore already routes this functionality through newer yarr/text paths.

### WREC cluster
Determine whether the entire `wrec` expectation can be removed from the restored Script baseline rather than emulated.

### Yarr cluster
Map missing historical `Regex*` files to the current `Yarr*` equivalents already used by WebKit.

## Immediate next execution step
Produce a **missing-path classification table** for the 56 missing references with columns such as:
- path
- cluster
- likely status (`obsolete`, `renamed`, `current equivalent exists`, `unclear`)
- proposed action

That table should drive the first real source-list surgery in `src/script/CMakeLists.txt`.

## Success condition for next pass
The next pass should aim to reduce the missing-path set from the current 56 by:
- removing clearly obsolete references
- replacing a first subset with current equivalents
- tightening the Script guard to reflect the reduced unresolved set
