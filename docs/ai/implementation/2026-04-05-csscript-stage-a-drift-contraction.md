# CsScript Stage A Drift Contraction — 2026-04-05

## Goal
Reduce the restored historical `CsScript` baseline's JavaScriptCore drift so the recovery path can move from "guarded off" toward "experimentally configurable".

## Changes made
### 1. Contracted the restored Script CMake expectations
Updated:
- `src/script/CMakeLists.txt`

The contraction pass did three kinds of work:

#### A. Safe path remaps
Applied straightforward path translations where the current embedded WebKit JavaScriptCore tree clearly exposes the equivalent artifact in a different location.

Examples:
- `runtime/MarkStack.cpp` -> `heap/MarkStack.cpp`
- `runtime/MarkStack.h` -> `heap/MarkStack.h`
- `runtime/MarkStackPosix.cpp` -> `heap/MarkStackPosix.cpp`
- `runtime/MarkStackWin.cpp` -> `heap/MarkStackWin.cpp`
- `runtime/StringBuilder.h` -> `wtf/text/StringBuilder.h`
- include root `JavaScriptCore/unicode` -> `JavaScriptCore/wtf/unicode`

#### B. Pruned clearly absent historical expectations
Removed references to historical JavaScriptCore-era files/subtrees absent from the current embedded tree.
Representative removals included:
- legacy `pcre/*`
- legacy `wrec/*`
- old `Regex*`-named yarr files
- legacy runtime-era files like `Collector.cpp`, `GlobalEvalFunction.cpp`, `JSImmediate.cpp`, `JSNumberCell.cpp`, `PrototypeFunction.cpp`
- obsolete generated artifacts like `generated/Grammar.cpp` / `generated/Grammar.h`

#### C. Reduced the path surface rather than inventing replacement code
This pass deliberately focused on build-contract narrowing in `src/script/CMakeLists.txt`, not on rewriting the Script implementation itself.
That keeps Stage A aligned with its current purpose:
- restore public/build visibility first
- defer deeper semantic adaptation until later compile-driven passes

## Quantified result
The generated JavaScriptCore drift audit changed from:
- **464** references total
- **408** existing
- **56** missing

to:
- **413** references total
- **413** existing
- **0** missing

Overlap with current `src/webkit/CMakeLists.txt` also improved slightly:
- from **147** overlapping references
- to **150** overlapping references

## Guard behavior change
Updated top-level `CMakeLists.txt` so the Script recovery guard no longer blocks `WITH_SCRIPT=ON` based on now-removed stale missing-path expectations.

Current behavior:
- `WITH_SCRIPT=ON` is allowed when the restored `src/script/` subtree and basic WebKit JavaScriptCore API presence exist
- BTK emits an **experimental recovery warning** instead of forcing Script back off

## Why this matters
This is the first pass where the recovered Script subsystem became path-consistent enough to be treated as an experimental configure-time component rather than only a guarded-off placeholder.

That is a real Stage A milestone.
