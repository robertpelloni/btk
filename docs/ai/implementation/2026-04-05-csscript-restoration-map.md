# CsScript Restoration Map — 2026-04-05

## Goal
Translate the selected historical Script baseline into a concrete first restoration map for BTK's current tree.

## Recovery source
Restore `CsScript` from:
- `85fd29b0d114fccb7b8eca371614bbb813ac0a01`

## Files/directories to recover first
### Build wiring
- `src/script/CMakeLists.txt`
- `src/script/api/api.cmake`
- `src/script/bridge/bridge.cmake`
- `src/script/parser/parser.cmake`

### Source subtree
- `src/script/api/`
- `src/script/bridge/`
- `src/script/parser/`

## Most important public headers expected by BML/declarative
These are the first visible compatibility targets because declarative public headers directly reference them:
- `src/script/api/qscriptvalue.h`
- `src/script/api/qscriptengine.h`
- `src/script/api/qscriptcontext.h`
- `src/script/api/qtscript.h`

## Most important private bridge files expected by declarative
These are the most important non-public files for getting `CsDeclarative` compiling again:
- `src/script/bridge/qscriptdeclarativeclass.cpp`
- `src/script/bridge/qscriptdeclarativeclass_p.h`
- `src/script/bridge/qscriptdeclarativeobject.cpp`
- `src/script/bridge/qscriptdeclarativeobject_p.h`
- `src/script/bridge/qscriptqobject.cpp`
- `src/script/bridge/qscriptqobject_p.h`

## Current-tree adaptation required
### Historical assumption
The historical Script build wiring expected JavaScriptCore under:
- `src/3rdparty/javascriptcore/...`

### Current reality
The current BTK tree exposes JavaScriptCore via WebKit paths such as:
- `src/3rdparty/webkit/Source/JavaScriptCore/...`
- `src/3rdparty/webkit/include/JavaScriptCore/...`

### First restoration mapping rule
For the first recovery pass, historical Script path references should be translated conceptually as:
- `src/3rdparty/javascriptcore/JavaScriptCore/<...>`
  -> `src/3rdparty/webkit/Source/JavaScriptCore/<...>`
- public forwarding/header-style `JavaScriptCore` includes
  -> use current `src/3rdparty/webkit/include/JavaScriptCore` and existing WebKit include roots where appropriate

## Current top-level build gaps that will still need work
Even after file recovery, BTK still needs top-level integration work because the current tree does not yet advertise Script as an optional component.

Likely follow-up edits will include:
- add `Script` back into top-level optional component handling
- decide whether `ScriptTools` should remain absent for the first recovery pass
- wire `src/script/` into the top-level build graph
- later decide whether package aliases such as `BTK::Script` should be exposed in parallel with legacy `CopperSpice::CsScript`

## Recommended first-pass restore order
### Step 1
Recover `src/script/` source and build files from `85fd29...`

### Step 2
Adapt historical JavaScriptCore paths in `src/script/CMakeLists.txt` to the current WebKit-based JavaScriptCore layout

### Step 3
Aim only for **Stage A public compatibility** first:
- header presence
- include generation
- top-level configure viability

### Step 4
Only after that, move to **Stage B declarative bridge/core compile** work:
- `QScriptDeclarativeClass`
- declarative object/context/list/value bridge classes

## Why this map is preferred
This keeps the project from making two common mistakes:
1. reviving too much at once (`CsScript` + `CsScriptTools` + declarative + imports)
2. attempting declarative buildability before the Script substrate is even visible again

The map preserves focus and keeps the BML revival incremental and testable.
