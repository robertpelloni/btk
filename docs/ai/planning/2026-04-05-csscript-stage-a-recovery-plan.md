# CsScript Stage A Recovery Plan — 2026-04-05

## Objective
Execute the first practical recovery stage for `CsScript` using the selected historical baseline while keeping scope tight enough to protect current build health.

## Baseline
- Recover from `85fd29b0d114fccb7b8eca371614bbb813ac0a01`

## Stage A scope
Stage A is intentionally limited to public compatibility and configure/build graph visibility.

### In scope
- restore `src/script/` directory structure
- restore Script public headers
- restore Script build wiring
- adapt the historical JavaScriptCore path assumptions to the current WebKit layout
- reintroduce top-level Script component handling as needed for configure-time visibility

### Out of scope for Stage A
- `CsScriptTools`
- full `CsDeclarative` compilation
- import/plugin revival
- debugger/tooling completeness

## Suggested execution order
1. recover historical `src/script/` files into the working tree
2. patch `src/script/CMakeLists.txt` path assumptions from old `javascriptcore` paths to current WebKit JavaScriptCore paths
3. decide whether top-level optional components should add `Script` immediately or whether Script should be revived behind a temporary local gate first
4. validate that public `QtScript` include generation is viable again
5. only then attempt broader compile work

## Stage A success criteria
- BTK contains a restored `src/script/` subtree again
- top-level configure can reason about Script explicitly rather than only through declarative failure
- Script public headers exist in-tree again
- the project is positioned to begin true compiler iteration on Script rather than only historical analysis

## Why this stage matters
This is the point where the BML revival effort stops being purely documentary.
It becomes an implementation program grounded in a specific historical baseline and a concrete restoration map.
