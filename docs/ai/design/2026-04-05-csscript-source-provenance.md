# CsScript Source Provenance Audit — 2026-04-05

## Purpose
Determine whether BTK must invent a new Script subsystem from scratch, or whether the repository itself already contains recoverable historical `CsScript` sources.

## Result
BTK does **not** need to start from zero.

The repository's own git history already contains:
- a full historical `src/script/` subtree
- a full historical `src/scripttools/` subtree
- prior CMake build wiring for both
- the private declarative bridge files needed by legacy declarative, including:
  - `qscriptdeclarativeclass.cpp`
  - `qscriptdeclarativeclass_p.h`
  - `qscriptdeclarativeobject.cpp`
  - `qscriptdeclarativeobject_p.h`

This materially changes the BML revival posture.
The next question is no longer "where could Script come from?"
It is now "which historical snapshot is the safest restoration base, and how much path/build adaptation is needed to make it work again in the current tree?"

## Key provenance findings
### Historical presence of `src/script/`
A `git log --all --stat -- src/script` audit shows a long-lived in-repo Script module lineage.

Important commits include:
- `4acd0d71e423cf10e8d3ce2f8b7fd7dcd13536e8` — CopperSpice 1.0.0 import, including the full Script subtree
- `fd59ad8fdcef85ee9517e7b768f0b7f5178bc68e` — added CMake build support for `src/script/`
- `4fb8e3486b0c63cefaeeb2e734a6b6b7762b3170` — removed `CsScript` build files
- `9a48596c0c14b34140e23903feae02a430335f32` — removed all `CsScript` source files

### Historical presence of `src/scripttools/`
A similar git audit shows that ScriptTools also existed in-tree for a long time.

Important commits include:
- `24653d638c507b5718d97bb37dc8b9b15b5dadf9` — added CMake build support for `src/scripttools/`
- `b71883ac547e1e62b0851c43c926ab2a8e319066` — removed `CsScriptTools` build files
- `60e2952b94841a5274ec67caeae56f12a4ff5a89` — removed `CsScriptTools` source

## Why this matters for BML
The earlier BML audit showed that legacy declarative depends on:
- public `QtScript` APIs
- private declarative/script bridge machinery
- some debugger/helper-era script support

The provenance audit now confirms that BTK's own history already contains the exact family of files that once satisfied those dependencies.

That strongly favors a **recovery-first** strategy over a **greenfield rewrite-first** strategy.

## Build-path adaptation finding
The old historical `src/script/CMakeLists.txt` did not build against today's exact third-party layout.
It expected a dedicated path family like:
- `src/3rdparty/javascriptcore/...`

In the current tree, the relevant JavaScriptCore sources live under WebKit, for example:
- `src/3rdparty/webkit/Source/JavaScriptCore/...`
- `src/3rdparty/webkit/include/JavaScriptCore/...`

This means historical Script restoration is plausible, but not literally drop-in.
A recovery pass will need to adapt include/source paths from the old standalone JavaScriptCore layout to the current WebKit-embedded JavaScriptCore layout.

## Architectural interpretation
### Good news
- the repo already contains recoverable `CsScript` source history
- the repo already contains recoverable `CsScriptTools` source history
- the repo already contains recoverable CMake wiring history
- the repo already contains the key private declarative bridge files BML needs

### Caution
- historical Script CMake expected a different JavaScriptCore layout
- historical ScriptTools is not required for the first BML core revival pass and should likely remain deferred
- restoration still requires careful adaptation to current BTK/CopperSpice build conventions

## Recommendation
The best next implementation step is **not** to invent a new Script subsystem blind.
The best next implementation step is:
1. recover the final pre-removal `src/script/` snapshot from git history
2. map the old JavaScriptCore paths to the current WebKit JavaScriptCore layout
3. attempt a Stage A public-compatibility restoration first
4. only then push into declarative bridge/core compilation

That is the lowest-risk and highest-leverage path now supported by evidence.
