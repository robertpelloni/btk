# CsScript Recovery Options — 2026-04-05

## Summary
The provenance audit shows BTK's own history contains a recoverable Script subsystem. That creates three practical recovery options.

## Option 1 — Restore the final pre-removal snapshot directly
Candidate baseline:
- the tree state immediately before:
  - `4fb8e3486b0c63cefaeeb2e734a6b6b7762b3170` (build file removal)
  - `9a48596c0c14b34140e23903feae02a430335f32` (source removal)

Pros:
- closest to the most recently maintained BTK/CopperSpice Script code
- likely matches later coding style and build conventions better than older history

Cons:
- still expects historical Script build assumptions that may need path/layout updates

## Option 2 — Restore from the first CMake-enabled Script era and forward-port
Candidate baseline:
- `fd59ad8fdcef85ee9517e7b768f0b7f5178bc68e`

Pros:
- very explicit module decomposition via `api.cmake`, `bridge.cmake`, `parser.cmake`
- useful as a reference for how the module was originally wired

Cons:
- older build style
- references a standalone `src/3rdparty/javascriptcore` layout that no longer exists in the same form

## Option 3 — Hybrid recovery
Recommended.

Approach:
- use the latest pre-removal Script source snapshot as the primary source base
- use older CMake-era commits as wiring/reference material where needed
- adapt the module to the current JavaScriptCore-under-WebKit layout
- defer ScriptTools until after core Script + Declarative viability is proven

Pros:
- best balance of freshness and recoverability
- fits BTK's compatibility-first migration style

Cons:
- requires a little more synthesis during restoration

## Critical adaptation issue
Historical Script CMake expected paths under:
- `src/3rdparty/javascriptcore/...`

Current tree reality is:
- `src/3rdparty/webkit/Source/JavaScriptCore/...`
- `src/3rdparty/webkit/include/JavaScriptCore/...`

This is the single most important concrete restoration delta discovered in this pass.

## Recommended restore order
1. recover `src/script/` source tree from the latest pre-removal snapshot
2. reconstruct minimal `src/script/CMakeLists.txt` build wiring
3. remap JavaScriptCore paths to the current WebKit layout
4. restore the public `QtScript` include tree
5. validate Stage A public compatibility
6. only then attempt declarative bridge/core compilation
7. defer `src/scripttools/` until later unless compiler errors prove it necessary earlier
