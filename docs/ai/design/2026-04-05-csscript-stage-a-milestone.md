# CsScript Stage A Milestone — 2026-04-05

## Milestone statement
BTK has now passed the first meaningful `CsScript` recovery milestone:

> The recovered historical Script subsystem is no longer merely present in-tree; it is now **path-consistent enough to configure experimentally** against the current WebKit-embedded JavaScriptCore tree.

## Why this is a milestone
Before this pass:
- the restored Script subtree existed
- but top-level Script enablement was still blocked by missing-path guard conditions rooted in historical JavaScriptCore assumptions

After this pass:
- those missing-path assumptions were contracted away from `src/script/CMakeLists.txt`
- the drift audit now reports **zero missing JavaScriptCore paths**
- `WITH_SCRIPT=ON` now reaches a successful configure pass with Script included in the BTK library set

## What this does and does not mean
### It means
- the historical baseline remains viable
- Stage A recovery is working as a narrowing/adaptation process
- the next frontier is compiler feedback, not path archaeology

### It does not mean
- `CsScript` is already build-clean
- Script is ready for downstream consumption
- declarative/BML can yet rely on Script as a completed subsystem

## Strategic consequence
This changes the next recommended work from:
- prune path drift
n
to:
- begin compile-driven `CsScript` diagnosis

That is a significant transition in maturity for the recovery effort.
