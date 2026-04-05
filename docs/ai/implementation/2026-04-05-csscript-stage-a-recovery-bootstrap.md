# CsScript Stage A Recovery Bootstrap — 2026-04-05

## Goal
Begin the actual Stage A `CsScript` recovery pass by reintroducing the historical Script subtree into the BTK working tree while preserving current configure/build health.

## Changes made
### 1. Restored historical `src/script/` subtree
Recovered `src/script/` from baseline:
- `85fd29b0d114fccb7b8eca371614bbb813ac0a01`

Recovered content includes:
- `src/script/CMakeLists.txt`
- `src/script/api/`
- `src/script/bridge/`
- `src/script/parser/`

Restored file count:
- **74 files** under `src/script/`

### 2. Reintroduced Script as a top-level optional BTK component
Updated top-level `CMakeLists.txt` so BTK now recognizes:
- `Script`
- `Declarative`

Current optional component ordering now places `Script` before `Declarative`, which is important for eventual declarative recovery because `CsDeclarative` depends on Script-era infrastructure.

### 3. Kept `Script` defaulted OFF for build safety
To avoid destabilizing the currently validated BTK build matrix, `WITH_SCRIPT` is currently defaulted to `OFF`, matching the safety posture already used for `Declarative`.

### 4. Added a configure-time Script viability guard
The restored historical `CsScript` baseline still expects an older JavaScriptCore layout and file set than the current WebKit-embedded tree provides.

So BTK now checks representative blockers when `WITH_SCRIPT=ON` is requested.
If they are missing, BTK:
- emits a warning
- explains the JavaScriptCore-era mismatch
- forces `WITH_SCRIPT=OFF` for that configure pass

This is intentionally analogous to the existing declarative guard.

### 5. Began JavaScriptCore path adaptation in `src/script/CMakeLists.txt`
The restored historical Script build expected:
- `src/3rdparty/javascriptcore/...`

The current tree provides JavaScriptCore via:
- `src/3rdparty/webkit/Source/JavaScriptCore/...`
- `src/3rdparty/webkit/include/JavaScriptCore/...`

The first bootstrap adaptation pass rewired the restored Script CMake file toward the current WebKit-based source root.

## Current state after bootstrap
### What is now true
- BTK once again contains a real `src/script/` subtree in the working tree
- top-level BTK recognizes Script as an optional component again
- Script is ordered before Declarative for future dependency correctness
- enabling Script no longer fails opaquely; it now produces an explicit warning describing the remaining JavaScriptCore mismatch
- default configure remains healthy

### What is not yet true
- `CsScript` is not yet buildable in the current tree
- the restored baseline still references older JavaScriptCore-era files absent from the current WebKit snapshot
- Stage A is therefore in **recovery bootstrap** status, not completion status

## Main remaining blockers identified by the guard
Representative missing historical expectations include:
- legacy `pcre`-based JavaScriptCore sources
- legacy `wrec` sources
- legacy runtime files such as `Collector.cpp`

These show that the next work item is not “find Script sources” anymore.
It is “adapt the recovered Script baseline to the specific JavaScriptCore variant currently embedded in BTK's WebKit tree.”

## Why this is still important progress
This pass changes Script revival from a purely documentary plan into an actual in-tree recovery bootstrap.
The project now has:
- restored historical Script sources
- restored Script build wiring
- top-level Script component visibility
- an honest compatibility guard preventing accidental breakage while the JavaScriptCore adaptation work remains unfinished
