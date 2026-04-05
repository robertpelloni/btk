# CsScript Linkage Unblock Milestone — 2026-04-05

## Summary
The restored `CsScript` Stage A track has now crossed another concrete milestone:

- the early engine-glue drift was reduced
- the first bridge/object-model drift was reduced
- the first explicit embedded-JavaScriptCore export/linkage blocker was reduced

The direct MSVC build no longer stops immediately in the restored Script wrapper layer or at the previously exposed `JSString.cpp` linkage mismatch.

## What changed architecturally

### Before this pass
The recovery effort had already moved beyond source-absence and path-drift problems, but it was still constrained by two closely stacked layers:
1. restored Script engine glue built against older JSC private APIs
2. restored Script bridge/object wrappers built against older JSC object/structure contracts

A separate export-definition mismatch also surfaced once enough of those layers were patched:
- `JSString.cpp` static metadata import/export failure

### After this pass
The direct build now survives those layers long enough to compile materially deeper embedded JavaScriptCore sources.

That means the current recovery frontier has moved from:
- shallow wrapper/API breakage

to:
- deeper whole-target compilation progress and whatever later blocker appears beyond it.

## Important design implication
This materially increases confidence in the recovery-first strategy for BML’s Script substrate.

Why:
- the repository-contained historical `CsScript` baseline is proving patchable against the current embedded JavaScriptCore tree
- the recovery work is no longer purely archaeological; it is now producing sustained forward build motion
- the remaining uncertainty is increasingly about later-stage build/runtime integration, not whether the restored Script layer is fundamentally viable

## New milestone reading
The key milestone is not “Script builds cleanly” yet.
The key milestone is:

> the recovered `CsScript` target now compiles far enough that its next blockers are later and more global than the original bridge/header collapse.

That is exactly the kind of frontier movement needed before a legacy declarative/BML revival can become realistic.

## Next architectural question
The next question is no longer whether Stage A recovery can continue.
It can.

The next question is:
- what is the first post-linkage-unblock fatal diagnostic once the build is allowed to run longer?

That answer will determine whether the next work item is:
- another localized Script compatibility patch, or
- a deeper embedded-JavaScriptCore target/configuration adjustment.
